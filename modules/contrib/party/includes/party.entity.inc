<?php

/**
 * @file
 * Contains the controller classes for Party entities.
 */

/**
 * The API controller class for the Party entity.
 */
class PartyController extends EntityAPIController {

  /**
   * Save a party.
   *
   * After the Party is saved we try and generate a label for it.
   */
  public function save($party) {
    // Make sure there's something to save so that drupal_write_record runs.
    if (empty($party->label)) {
      $party->label = "Not yet labelled";
    }

    if (isset($party->pid)) {
      drupal_write_record('party', $party, 'pid');
      field_attach_update('party', $party);
      module_invoke_all('entity_update', $party, 'party');
    }
    else {
      drupal_write_record('party', $party);
      field_attach_insert('party', $party);
      module_invoke_all('entity_insert', $party, 'party');
    }

    // Set the party label
    $this->setLabel($party);
    return $party;
  }

  /**
   * Set the label on a Party object using the first label plugin
   * (when ordered by weight) that returns a non-empty value.
   *
   * This method saves the label to the database as well as generating it,
   * this allows the function to be called independantly of party save. Also,
   * as most Party label plugins require the party to be saved to function
   * correctly, this avoids having to go through the Party::save() method
   * multiple times.
   *
   * @param $party
   *  A party object.
   */
  public function setLabel($party) {
    module_load_include('inc', 'party', 'party.admin');

    // Get the label plugins.
    $label_plugins = party_settings_get_party_label_plugins();

    // Try each plugin, lowest weight first, until a plugin is
    // found that returns a non-empty value.
    foreach ($label_plugins as $path => $label_plugin) {
      $label_callback = $label_plugin['label callback'];
      $label = $label_callback($party);
      if (!empty($label)) {
        $party->label = $label;
        break;
      }
    }

    // No label plugins returned a label, use the party id.
    if (empty($label)) {
      $party->label = $party->id;
    }

    // Save the label to the database.
    // @todo: if drupal_write_record incurs too much overhead replace with a
    // simple update query.
    drupal_write_record('party', $party, 'pid');
  }

  /**
   * Overriding the buildContent function to add entity specific fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    return $content;
  }
}

/**
 * The class used for party entities.
 */
class Party extends Entity {

  /**
   * The internal, numeric party-id.
   *
   * @var integer
   */
  public $pid;

  /**
   * The human-readable label for the party.
   *
   * @var string
   *
   * @see PartyController::setLabel()
   */
  public $label;

  /**
   * Boolean representing whether this party has been merged into another
   * party.
   *
   * @var integer
   */
  public $merged;

  /**
   * If $merged is TRUE, this is the pid of the party that this party has been
   * merged into.
   *
   * @var integer
   */
  public $merged_party;

  public function __construct($values = array()) {
    parent::__construct($values, 'party');
  }

  /**
   * Return the default label for this party object.
   *
   * @return string
   *   Human readable label for the party object.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Return the default path for this party.
   *
   * @return array
   *   An array with the following key:
   *   - path - the drupal path for the party.
   */
  protected function defaultUri() {
    return array('path' => 'party/' . $this->pid);
  }
}
