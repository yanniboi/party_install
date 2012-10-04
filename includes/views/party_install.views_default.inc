<?php

/**
 * Implements hook_views_default_views().
 */

function party_install_views_default_views() {
  // Export of view from site.
  $view = new view();
  $view->name = 'party_selection';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'party';
  $view->human_name = 'Party Selection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Person */
  $handler->display->display_options['relationships']['eid_profile2']['id'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['ui_name'] = 'Person';
  $handler->display->display_options['relationships']['eid_profile2']['label'] = 'Individual';
  $handler->display->display_options['relationships']['eid_profile2']['data_set'] = 'profile2_individual';
  $handler->display->display_options['relationships']['eid_profile2']['main'] = 0;
  /* Relationship: Student */
  $handler->display->display_options['relationships']['eid_profile2_1']['id'] = 'eid_profile2_1';
  $handler->display->display_options['relationships']['eid_profile2_1']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2_1']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2_1']['ui_name'] = 'Student';
  $handler->display->display_options['relationships']['eid_profile2_1']['label'] = 'Student';
  $handler->display->display_options['relationships']['eid_profile2_1']['data_set'] = 'profile2_student';
  $handler->display->display_options['relationships']['eid_profile2_1']['main'] = 0;
  /* Relationship: Staff */
  $handler->display->display_options['relationships']['eid_profile2_2']['id'] = 'eid_profile2_2';
  $handler->display->display_options['relationships']['eid_profile2_2']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2_2']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2_2']['ui_name'] = 'Staff';
  $handler->display->display_options['relationships']['eid_profile2_2']['label'] = 'Staff';
  $handler->display->display_options['relationships']['eid_profile2_2']['data_set'] = 'profile2_staff';
  $handler->display->display_options['relationships']['eid_profile2_2']['main'] = 0;
  /* Relationship: Organisation */
  $handler->display->display_options['relationships']['eid_profile2_3']['id'] = 'eid_profile2_3';
  $handler->display->display_options['relationships']['eid_profile2_3']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2_3']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2_3']['ui_name'] = 'Organisation';
  $handler->display->display_options['relationships']['eid_profile2_3']['label'] = 'Organisation';
  $handler->display->display_options['relationships']['eid_profile2_3']['data_set'] = 'profile2_organisation';
  $handler->display->display_options['relationships']['eid_profile2_3']['main'] = 0;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'party';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = '';
  $handler->display->display_options['fields']['pid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
  /* Field: Party: Hats */
  $handler->display->display_options['fields']['party_hat']['id'] = 'party_hat';
  $handler->display->display_options['fields']['party_hat']['table'] = 'field_data_party_hat';
  $handler->display->display_options['fields']['party_hat']['field'] = 'party_hat';
  $handler->display->display_options['fields']['party_hat']['label'] = '';
  $handler->display->display_options['fields']['party_hat']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['party_hat']['delta_offset'] = '0';
  /* Field: Party: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'party';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = '';
  $handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid_1']['id'] = 'pid_1';
  $handler->display->display_options['fields']['pid_1']['table'] = 'party';
  $handler->display->display_options['fields']['pid_1']['field'] = 'pid';
  /* Sort criterion: Party: Label */
  $handler->display->display_options['sorts']['label']['id'] = 'label';
  $handler->display->display_options['sorts']['label']['table'] = 'party';
  $handler->display->display_options['sorts']['label']['field'] = 'label';
  /* Contextual filter: Party: Hats (party_hat) */
  $handler->display->display_options['arguments']['party_hat_hat_name']['id'] = 'party_hat_hat_name';
  $handler->display->display_options['arguments']['party_hat_hat_name']['table'] = 'field_data_party_hat';
  $handler->display->display_options['arguments']['party_hat_hat_name']['field'] = 'party_hat_hat_name';
  $handler->display->display_options['arguments']['party_hat_hat_name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['party_hat_hat_name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['party_hat_hat_name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['party_hat_hat_name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['party_hat_hat_name']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['party_hat_hat_name']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['party_hat_hat_name']['limit'] = '0';
  /* Filter criterion: Party: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'party';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['operator'] = 'allwords';
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Profile: Course (field_student_course) */
  $handler->display->display_options['filters']['field_student_course_value']['id'] = 'field_student_course_value';
  $handler->display->display_options['filters']['field_student_course_value']['table'] = 'field_data_field_student_course';
  $handler->display->display_options['filters']['field_student_course_value']['field'] = 'field_student_course_value';
  $handler->display->display_options['filters']['field_student_course_value']['relationship'] = 'eid_profile2_1';
  $handler->display->display_options['filters']['field_student_course_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_student_course_value']['expose']['operator_id'] = 'field_student_course_value_op';
  $handler->display->display_options['filters']['field_student_course_value']['expose']['label'] = 'Course (field_student_course)';
  $handler->display->display_options['filters']['field_student_course_value']['expose']['operator'] = 'field_student_course_value_op';
  $handler->display->display_options['filters']['field_student_course_value']['expose']['identifier'] = 'field_student_course_value';
  $handler->display->display_options['filters']['field_student_course_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_student_course_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_student_course_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Profile: Department (field_staff_department) */
  $handler->display->display_options['filters']['field_staff_department_value']['id'] = 'field_staff_department_value';
  $handler->display->display_options['filters']['field_staff_department_value']['table'] = 'field_data_field_staff_department';
  $handler->display->display_options['filters']['field_staff_department_value']['field'] = 'field_staff_department_value';
  $handler->display->display_options['filters']['field_staff_department_value']['relationship'] = 'eid_profile2_2';
  $handler->display->display_options['filters']['field_staff_department_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['operator_id'] = 'field_staff_department_value_op';
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['label'] = 'Department';
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['operator'] = 'field_staff_department_value_op';
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['identifier'] = 'field_staff_department_value';
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_staff_department_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Profile: Gender (field_individual_gender) */
  $handler->display->display_options['filters']['field_individual_gender_value']['id'] = 'field_individual_gender_value';
  $handler->display->display_options['filters']['field_individual_gender_value']['table'] = 'field_data_field_individual_gender';
  $handler->display->display_options['filters']['field_individual_gender_value']['field'] = 'field_individual_gender_value';
  $handler->display->display_options['filters']['field_individual_gender_value']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['field_individual_gender_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['operator_id'] = 'field_individual_gender_value_op';
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['label'] = 'Gender';
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['operator'] = 'field_individual_gender_value_op';
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['identifier'] = 'field_individual_gender_value';
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_individual_gender_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Profile: Nationality (field_individual_nationality) */
  $handler->display->display_options['filters']['field_individual_nationality_tid']['id'] = 'field_individual_nationality_tid';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['table'] = 'field_data_field_individual_nationality';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['field'] = 'field_individual_nationality_tid';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['value'] = '';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['operator_id'] = 'field_individual_nationality_tid_op';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['label'] = 'Nationality';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['operator'] = 'field_individual_nationality_tid_op';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['identifier'] = 'field_individual_nationality_tid';
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_individual_nationality_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['field_individual_nationality_tid']['vocabulary'] = 'nationality';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  // Add it to the views array.
  $views[$view->name] = $view;

  // Return the views array.
  return $views;
}
