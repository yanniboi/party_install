<?php

/**
 * @file
 *
 * Sample plugin to output just the party id as a label.
 * This is just a proof of concept / example.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 *
 * @todo: document our plugin settings somewhere!
 *  - 'title': The admin name of the plugin.
 *  - 'description': Description for the admin UI.
 *  - 'label callback': The callback to use for generating a label.
 *    The signature is $callback($party).
 *  - 'options form callback': The callback for the settings form.
 *    Eg, for a FieldAPI label plugin this would let you choose which field(s)
 *    to take the label text from.
 */
$plugin = array(
  'title' => t("Attached Entity Field"),
  'description' => t('Form a label from a Field on an Attached Entity.'),
  'label callback' => 'party_attached_entity_field_label',
  'options form callback' => 'party_attached_entity_field_options_form',
);

/**
 * Generate the label for a party.
 */
function party_attached_entity_field_label($party) {
  if (empty($party->pid)) {
    return;
  }

  $data_set_name = variable_get('party_name_label_data_set', 'party');
  $field_name = variable_get('party_name_label_field', FALSE);
  if ($data_set_name != 'party') {
    $data_set_controller = party_get_crm_controller($party, $data_set_name);
    $entity = $data_set_controller->getEntity();
    $entity_type = $data_set_controller->getDataInfo('entity type');
    $entity_bundle = $data_set_controller->getDataInfo('entity bundle');
  }
  else {
    $entity = $party;
    $entity_type = 'party';
    $entity_bundle = 'party';
  }

  $return = FALSE;

  if ($entity) {
    // Get our field and instance settings
    $field = field_info_field($field_name);
    $instance = field_info_instance($entity_type, $field_name, $entity_bundle);
    $instance['display']['default']['label'] = 'hidden';

    // Get the field items and limit them to one item
    $items = field_get_items($entity_type, $entity, $field_name);

    // If there are no items return empty.
    if (!$items) {
      return;
    }

    $items = array(reset($items));

    // Get our rendered version of it
    $return = drupal_render(field_default_view($entity_type, $entity, $field, $instance, LANGUAGE_NONE, $items, 'default'));

    // Convert html entities back into real input
    $return = html_entity_decode(strip_tags($return));
  }

  return $return;
}

/**
 * the options form
 */
function party_attached_entity_field_options_form($form, &$form_state) {
  $default_data_set = variable_get('party_name_label_data_set', isset($form_state['values']['data_set_name']) ? $form_state['values']['data_set_name'] :'party');
  $default_field = variable_get('party_name_label_field', isset($form_state['values']['field_name']) ? $form_state['values']['field_name'] : FALSE);

  // Set the file to be loaded
  $form_state['build_info']['files'] = array(drupal_get_path('module', 'party'). '/plugins/party_name_label/attached_entity_field.inc');

  // Get Data Sets
  $sets = party_get_data_set_info();

  $options = array(
    'party' => 'Party Entity',
  );
  foreach ($sets as $data_set_name => $definition) {
    $options[$data_set_name] = $definition['label'];
  }

  $form['data_set_name'] = array(
    '#type' => 'select',
    '#title' => t('Data Set'),
    '#default_value' => $default_data_set,
    '#options' => $options,
    '#weight' => 1,
    '#ajax' => array(
      'wrapper' => 'field-input',
      'callback' => 'party_attached_entity_field_field_options',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $field_options = array();
  if ($default_data_set == 'party') {
    // Do something to set the bundle
    $fields = field_info_instances('party', 'individual');
    foreach ($fields as $field) {
      $field_options[$field['field_name']] = $field['label'];
    }
  }
  else {
    $data_set_info = $sets[$default_data_set];
    $fields = field_info_instances($data_set_info['entity type'], $data_set_info['entity bundle']);
    foreach ($fields as $field) {
      $field_options[$field['field_name']] = $field['label'];
    }
  }

  if (empty($field_options)) {
    $field_options[0] = 'No Fields';
  }

  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#default_value' => $default_field,
    '#options' => $field_options,
    '#weight' => 2,
    '#prefix' => '<div id="field-input">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function party_attached_entity_field_field_options($form, $form_state) {
  return $form['field_name'];
}

function party_attached_entity_field_options_form_submit($form, &$form_state) {
  variable_set('party_name_label_data_set', $form_state['values']['data_set_name']);
  variable_set('party_name_label_field', $form_state['values']['field_name']);
}
