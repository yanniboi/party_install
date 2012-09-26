<?php
/**
 * @file
 * Enables form alters for a party site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function party_install_form_install_configure_form_alter(&$form, &$form_state, $form_id) {
  // Pre-populate the site name with the server name.
  // $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];

  // Hide some messages from various modules that are just too chatty!
  drupal_get_messages('status');
  drupal_get_messages('warning');

  // Set reasonable defaults for site configuration form
  $form['site_information']['site_name']['#default_value'] = 'Party';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  // What is the default value for London?
  $form['server_settings']['site_default_country']['#default_value'] = 'GB';
  $form['server_settings']['date_default_timezone']['#default_value'] = 'Europe/London'; // The Party happens in the North West though!! 

}

function party_install_form_install_settings_form_alter(&$form, &$form_state, $form_id) {
  print 'cheese';
}

function hook_install_tasks_alter(&$tasks, $install_state) {
  dpm($tasks);
  drupal_set_message(print_r($tasks, true));
  print 'form cheese';
}

/**
 * Implements hook_install_tasks().
 */
function party_install_install_tasks($install_state) {
  $country_is_us = !empty($install_state['parameters']['country']) && $install_state['parameters']['country'] = 'US'; 
  $country_is_ca = !empty($install_state['parameters']['country']) && $install_state['parameters']['country'] = 'CA';
  $tasks = array(
    'party_create' => array(
      'display_name' => st('Create Parties'),
      'type' => 'form',
      'run' => 'INSTALL_TAST_RUN_IF_NOT_COMPLETED',
      'function' => 'party_install_party_generate_form',
    ),
  );
  return $tasks;
}

function party_install_party_generate_form($form, &$form_state, &$install_state) {
  drupal_set_title(st('Create Parties'));

  $form['markup'] = array(
    '#type' => 'markup',
    '#markup' => st('You can choose to create some "dummy" parties.'),
  );
  $form['entry'] = array(
    '#title' => 'Create Parties',
    '#type' => 'textfield',
    '#description' => st('Please enter how many parties you would like to create.'),
    '#default_value' => 5,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit', 
    '#value' => st('Save and continue'), 
    '#weight' => 15,
  );

//  drupal_set_message(print_r($form_state, true));
//  drupal_set_message(print_r($install_state, true));
  return $form;
}

function party_install_party_generate_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['entry'])) {
    form_error($form['entry'], 'Field must be a number!');
  }
  elseif ($form_state['values']['entry'] <= 0) {
    form_error($form['entry'], 'Field must be a postitive number!');
  }
 
}

function party_install_party_generate_form_submit($form, &$form_state) {
  $i = 1;

  while ($i <= $form_state['values']['entry']) {
    $party = array();
    $party = party_create($party);

    // Parties are dummy content so do not need to be merged
    $party->merged = 0;
    $party->language = LANGUAGE_NONE;

    // Generate dummy content for fields
    module_load_include('inc', 'devel_generate', 'devel_generate.fields');
    devel_generate_fields($party, 'party', 'party');

    // save the party
    $party->save();
    $i++;
  }
}

