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

  dpm($form);

  // Set reasonable defaults for site configuration form
  $form['site_information']['site_name']['#default_value'] = 'Party';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  // What is the default value for London?
//  $form['server_settings']['site_default_country']['#default_value'] = 'United Kingdom';
//  $form['server_settings']['date_default_timezone']['#default_value'] = 'Europe/London'; // The Party happens in the North West though!! 

}

function hook_install_tasks_alter(&$tasks, $install_state) {
  dpm($forms);
  drupal_set_message(print_r($forms, true));
  print 'form cheese';
}
