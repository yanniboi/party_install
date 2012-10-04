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

/**
 * Implements hook_install_tasks().
 */
function party_install_install_tasks($install_state) {
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

/**
 * This function builds the form for the Create Party Install Task.
 */
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
    module_load_include('inc', 'devel_generate', 'devel_generate');
    module_load_include('inc', 'devel_generate', 'devel_generate.fields');
    devel_generate_fields($party, 'party', 'party');

    // Save the party
    $party->save();

    // Load the party hats
    $individual = party_hat_load('individual');
    $student = party_hat_load('student');
    $staff = party_hat_load('staff');
    $organisation = party_hat_load('organisation');

    $hat_list = array(
      array($organisation),
      array($individual),
      array($individual, $staff),
      array($individual, $student),
      array($individual, $staff, $student),
    );
    unset($party->party_hat);

    // Assign random hat
    $hats = $hat_list[mt_rand(0, 4)];
    party_hat_assign_hats($party, $hats);

    // Attach a selection of data sets
    $data_sets = party_get_party_data_sets($party);

    // Loop through all the data sets that may be attached to the party
    foreach($data_sets as $id => $name) {
      $data_set_controller = party_get_crm_controller($party, $name);
      $entity = $data_set_controller->getEntity(0, true);

      // Devel generate requires that a language be set.
      if (!isset($entity->language)) {
        $entity->language = LANGUAGE_NONE;
      }
      // Use Devel generate to generate content for all fields 'attached' to party
      module_load_include('inc', 'devel_generate', 'devel_generate');
      module_load_include('inc', 'devel_generate', 'devel_generate.fields');
      devel_generate_fields($entity, $data_set_controller->getDataInfo('entity type'), $data_set_controller->getDataInfo('entity bundle'));

      // If 'field_main_name' exists label it
      $current_hat = party_hat_get_hats($party);

      // If party has organisation hat use party_generate_organisation_label
      if (field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_main_name', $data_set_controller->getDataInfo('entity bundle')) && isset($current_hat['organisation'])) {
        $entity->field_main_name[LANGUAGE_NONE][0]['value'] = party_install_generate_organisation_label();
      }
      // If party doesn't have organisation hat use party_generate_label
      elseif (field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_main_name', $data_set_controller->getDataInfo('entity bundle'))) {
        $entity->field_main_name[LANGUAGE_NONE][0]['value'] = party_install_generate_label();
      }

      // If 'field_main_address' exists the use party_generate_address
      if (field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_main_address', $data_set_controller->getDataInfo('entity bundle'))) {
        $entity->field_main_address[LANGUAGE_NONE][0]['value'] = party_install_generate_address();
      }
      // If 'field_main_email' exists the use party_generate_email
      /* if (field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_main_email', $data_set_controller->getDataInfo('entity bundle'))) {
        $entity->field_main_email[LANGUAGE_NONE][0]['value'] = party_install_generate_email();
      }*/

      // If 'field_individual_photo' exists then use party_generate_image
      // Load the field defintion.
      $field = field_info_field('field_individual_photo');
      if ($instance = field_info_instance($data_set_controller->getDataInfo('entity type'), 'field_individual_photo', $data_set_controller->getDataInfo('entity bundle'))) {
        //unset($entity->field_image);
        //$entity->field_image[LANGUAGE_NONE][] = party_generate_image($field, $instance);
        $entity->field_individual_photo['und'][0] = party_install_generate_image($field, $instance);
      }

      // Save changes
      $data_set_controller->save();
      entity_save($data_set_controller->getDataInfo('entity type'), $entity);
      $party->save();
    }
    $i++;
  }
}

/**
 * This function generates random names for dummy parties.
 */
function party_install_generate_label() {

 $first_name = array("Addie", "Aida", "Allie", "Amanda", "Anita", "Anne", "Audie", "Augusta", "Barb", "Barry", "Bea", "Ben", "Bess", "Celine", "Chris", "Constance", "Dave", "Eileen", "Frank", "Grace", "Harriet", "Hazel", "Hope", "Hugh", "Isabelle", "Ivana", "Jessica", "John", "Josie", "Liz", "Lois", "Luke", "Lynn", "Manny", "Mark", "Marsha", "Martin", "Maureen", "Minnie", "Missy", "Olive" ,"Paige" ,"Patty", "Peg", "Phillip", "Randy", "Ray", "Reeve", "Rhoda", "Rita", "Suzie", "Teri", "Tex", "Theresa");

  $second_name = array("Anderson", "Andrews", "Brown", "Campbell", "Clark", "Clarke", "Davies", "De Wet", "Du Toit", "Edwards", "Evans", "Garcia", "Green", "Hall", "Harris", "Hughes", "Jackson", "Johnson", "Jones", "Khan", "Kumar", "Lewis", "Macdonald", "Martin", "Martinez", "Miller", "Mitchell", "Moore", "Morrison", "Mumford", "Murray", "Patak", "Patel", "Patel", "Paterson", "Reid", "Roberts", "Robertson", "Robinson", "Rogers", "Ross", "Scott", "Smith", "Stewart", "Taylor", "Thomas", "Thompson", "Walker", "Watson", "White", "Williams", "Wilson", "Wright", "Young");

  $num_first_name = count($first_name);
  $num_second_name = count ($second_name);

  $name = $first_name[mt_rand(0, $num_first_name - 1)] . ' ' . $second_name[mt_rand(0, $num_second_name - 1)];

  return $name;
}

/**
 * This function generates random organisation names for dummy parties.
 */
function party_install_generate_organisation_label() {
  $city = array("Manchester", "Birmingham", "London", "Oxford", "Belfast", "Bristol", "Dubai");
  $industry = array("Banking", "Bakers", "Public Relations", "Events", "Sports", "Painters");
  $industry_type = array("LTD", "Limited", "Incorporated", "and Co", "Cooperative", "PLC");

  $num_city = count($city);
  $num_industry = count($industry);
  $num_industry_type = count($industry_type);

  $organisation = $city[mt_rand(0, $num_city - 1)] . ' ' . $industry[mt_rand(0, $num_industry - 1)] . ' ' . $industry_type[mt_rand(0, $num_industry_type - 1)];

  return $organisation;
}

/**
 * This function generates random addresses for dummy parties.
 */
function party_install_generate_address() {
  $street_name = array("Anderson", "Andrews", "Brown", "Campbell", "Clark", "Clarke", "Davies", "De Wet", "Du Toit", "Edwards", "Evans", "Garcia", "Green", "Hall", "Harris", "Hughes", "Jackson", "Johnson", "Jones", "Khan", "Kumar", "Lewis", "Macdonald", "Martin", "Martinez", "Miller");
  $street = array("Street", "Road", "Cul-de-sac", "Drive", "Ave", "Crescent", "Fold");
  $city = array("Manchester", "Birmingham", "London", "Oxford", "Belfast", "Bristol", "Dubai");

  $num_street_name = count($street_name);
  $num_street = count($street);
  $num_city = count($city);

  $address = mt_rand(1, 100) . ' ' . $street_name[mt_rand(0, $num_street_name - 1)] . ' ' . $street[mt_rand(0, $num_street - 1)] . ', ' . $city[mt_rand(0, $num_city - 1)];

  return $address;
}

/**
 * This function generates random images for dummy parties.
 */
function party_install_generate_image($field, $instance) {

  $my_images = array('1.jpg', '2.jpg', '3.jpg', '4.jpg', '5.jpg', '6.jpg', '7.jpg', '8.jpg', '9.jpg', '10.jpg', '11.jpg', '12.jpg');

  if ($path = DRUPAL_ROOT . '/profiles/party_install/images/' . $my_images[mt_rand(0, 9)]) {
    $source = new stdClass();
    $source->uri = $path;
    $source->uid = 1; // TODO: randomize? Use case specific.
    $source->filemime = 'image/' . pathinfo($path, PATHINFO_EXTENSION);
    $source->filename = array_pop(explode("//", $path));
    // $destination_dir = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];
    $destination_dir = 'public://';
    file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);

    // $tmp_file = drupal_tempnam('temporary://', 'imagefield_');
    // $destination = $tmp_file . '.jpg';

    $destination = $destination_dir . '/' . devel_create_greeking(4) . basename($path);
    $file = file_copy($source, $destination, FILE_CREATE_DIRECTORY);
    $object_field['fid'] = $file->fid;
    $object_field['alt'] = devel_create_greeking(4);
    $object_field['title'] = devel_create_greeking(4);
    drupal_set_message('greeking done');
  }
  else {
    drupal_set_message('Image file save failed');
    return FALSE;
  }

  return $object_field;
}

/**
 * Implements hook_views_api()
 */

function party_install_views_api() {
  return array(
    'api' => 3,
    'path' => drupal_get_path('profile', 'party_install') . '/includes/views',
//    'template path' => drupal_get_path(),
  );
}
