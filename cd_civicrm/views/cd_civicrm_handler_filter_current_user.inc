<?php

/**
 * @file
 * Views handlers for cd_civicrm.
 *
 * @todo Change the block/page title appropriately.
 */

/**
 * Filter handler for the current user's State.
 *
 * @ingroup views_field_handlers
 */
class cd_civicrm_handler_filter_state_current_user extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t("Is the logged in user's state");
  }

  function query() {
    $this->ensure_my_table();
    $where = "$this->table_alias.$this->real_field ";

    if (empty($this->value)) {
      $where .= '<> ***CURRENT_USERS_STATE***';
      if ($this->accept_null) {
        $where = '(' . $where . " OR $this->table_alias.$this->real_field IS NULL)";
      }
    }
    else {
      $where .= '= ***CURRENT_USERS_STATE***';
    }
    $this->query->add_where($this->options['group'], $where);
  }
}

/**
 * Filter handler for the current user's District.
 *
 * @ingroup views_field_handlers
 */
class cd_civicrm_handler_filter_district_current_user extends views_handler_filter {

  function value_form(&$form, &$form_state) { }

  function can_expose() {
    return FALSE;
  }

  function query() {
    $this->ensure_my_table();

    // Remember that senators have a CD of 0.
    $where = "$this->table_alias.$this->real_field IN (***CURRENT_USERS_DISTRICT***, 0)";

    $this->query->add_where($this->options['group'], $where);
  }
}
