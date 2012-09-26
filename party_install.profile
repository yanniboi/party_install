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
    'geo_country' => array(
      'display_name' => st('Choose a country'),
    ),
    'geo_state' => array(
      'display_name' => st('Choose a state or province'),
      'display' => $country_is_us || $country_is_ca,
      'type' => 'form',
      'run' => $country_is_us || $country_is_ca ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
      'function' => $country_is_us ? 'geo_state_form' : 'geo_province_form',
    ),
  );
  return $tasks;
}
