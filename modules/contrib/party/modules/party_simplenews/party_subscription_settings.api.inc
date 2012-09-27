<?php
/**
 * @file party_subscription_settings.api.inc
 *   Helper functions etc for the subscription settings
 */

 /**
 * Fetch a party_subscription_settings object. Make sure that the wildcard you choose
 * in the party_subscription_settings entity definition fits the function name here.
 *
 * @param $subscription_settings_id
 *   Integer specifying the party_subscription_settings id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $party_subscription_settings object or FALSE if it cannot be loaded.
 *
 * @see party_subscription_settings_load_multiple()
 */
function party_subscription_settings_load($subscription_settings_id, $reset = FALSE) {
  $party_subscription_settingss = party_subscription_settings_load_multiple(array($subscription_settings_id), array(), $reset);
  return reset($party_subscription_settingss);
}


/**
 * Load multiple party_subscription_settingss based on certain conditions.
 *
 * @param $subscription_settings_ids
 *   An array of party_subscription_settings IDs.
 * @param $conditions
 *   An array of conditions to match against the {party_subscription_settings} table.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of party_subscription_settings objects, indexed by subscription_settings_id.
 *
 * @see entity_load()
 * @see party_subscription_settings_load()
 */
function party_subscription_settings_load_multiple($subscription_settings_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('party_subscription_settings', $subscription_settings_ids, $conditions, $reset);
}


/**
 * Deletes a party_subscription_settings.
 */
function party_subscription_settings_delete(PartySubscriptionSettings $party_subscription_settings) {
  $party_subscription_settings->delete();
}


/**
 * Delete multiple party_subscription_settingss.
 *
 * @param $subscription_settings_ids
 *   An array of party_subscription_settings IDs.
 */
function party_subscription_settings_delete_multiple(array $subscription_settings_ids) {
  entity_get_controller('party_subscription_settings')->delete($subscription_settings_ids);
}


/**
 * Create a party_subscription_settings object.
 */
function party_subscription_settings_create($values = array()) {
  return entity_get_controller('party_subscription_settings')->create($values);
}


/**
 * Saves a party_subscription_settings to the database.
 *
 * @param $party_subscription_settings
 *   The party_subscription_settings object.
 */
function party_subscription_settings_save(PartySubscriptionSettings $party_subscription_settings) {
  return $party_subscription_settings->save();
}

/**
 * Get the party from the party subscription settings entity
 */
function party_subscription_settings_get_party(PartySubscriptionSettings $party_subscription_settings) {
  $id = $party_subscription_settings->subscription_settings_id;

  $result = db_select('party_attached_entity', 'pae')
    ->fields('pae', array('pid'))
    ->condition('eid', $id, '=')
    ->condition('data_set', 'subscription_settings')
    ->execute()
    ->fetchCol();

  $party_id = reset($result);
  return party_load($party_id);
}

/**
 * Add a subscription settings attached entity
 */
function party_simplenews_add_subsciption_settings_attached_entity($party) {
  party_get_data_set($party, 'subscription_settings', TRUE);
}

/**
 * Party subscription settings edit form
 */
function party_subscription_settings_edit_form($form, &$form_state, &$attached_entity, $party) {
  if (empty($party->pid)) {
    return FALSE;
  }

  $simplenews_form = array();
  // Get all the emails on the party.
  $emails = party_get_all_emails($party);

  foreach ($emails as $key => $email) {
    $emails[$key] .= ' (Primary)';
    break;
  }

  $hash = 'subscription_settings:' . $attached_entity->subscription_settings_id;
  $current_ref = !empty($form_state['values'][$hash]['field_ref']) ? $form_state['values'][$hash]['field_ref'] : reset(array_keys($emails));
  if (!empty($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == $hash . '[field_ref]') {
    unset($form_state['values'][$hash]);
    unset($form_state['input'][$hash]);
  }

  $simplenews_form['field_ref'] = array(
    '#type' => 'select',
    '#title' => t('Email'),
    '#options' => $emails,
    '#default_value' => $current_ref,
    '#ajax' => array(
      'callback' => 'party_subscription_settings_edit_form_ajax',
      'wrapper' => 'simplenews-subscriptions',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );

  module_load_include('inc', 'simplenews', 'includes/simplenews.subscription');
  $subscriber = party_simplenews_subscriber_load_by_pid($party->pid, $current_ref);
  // If this email doesn't have a subscriber yet, create one
  if (!$subscriber) {
    $subscriber = new stdClass();
    $subscriber->mail = $emails[$current_ref];
    $subscriber->activated = 1;
    $subscriber->language = '';
    $subscriber->uid = 0;
    $subscriber->party_id = $party->pid;
    $subscriber->party_field_ref = $current_ref;

    simplenews_subscriber_save($subscriber);
  }

  simplenews_subscriptions_account_form($simplenews_form, $form_state, $subscriber);
  unset($simplenews_form['#submit']);

  $simplenews_form['subscriptions']['#prefix'] = '<div id="simplenews-subscriptions">';
  $simplenews_form['subscriptions']['#suffix'] = '</div>';

  return $simplenews_form;
}

function party_subscription_settings_edit_form_ajax($form, $form_state) {
  $hash = FALSE;
  // Now we have to hunt.
  foreach($form_state['#attached_entities'] as $key => $attached_entity) {
    if ($attached_entity->data_set_name == 'subscription_settings') {
      $hash = $key;
      break;
    }
  }
  return $form[$hash]['subscriptions'];
}

/**
 * Submit party subscription settings form
 */
function party_subscription_settings_edit_form_submit($form, &$form_state, &$attached_entity, $party) {
  $hash = 'party_subscription_settings:' . $attached_entity->subscription_settings_id;
  $emails = party_get_all_emails($party);
  $values = &$form_state['values'][$hash];

  $field_ref = $values['field_ref'];
  $mail = $emails[$field_ref];
  // We first subscribe, then unsubscribe. This prevents deletion of subscriptions
  // when unsubscribed from the
  arsort($values['subscriptions']['newsletters'], SORT_NUMERIC);
  foreach ($values['subscriptions']['newsletters'] as $tid => $checked) {
    if ($checked) {
      // We may want to override this to make sure we get the right field ref
      simplenews_subscribe_user($mail, $tid, FALSE, 'website');
    }
    else {
      simplenews_unsubscribe_user($mail, $tid, FALSE, 'website');
    }
  }

  drupal_set_message(t('The newsletter subscriptions for %party &lt;%email&gt; have been updated.', array('%party' => $party->label, '%email' => $mail)));
}
