<?php

/**
 * Commerce option set form callback
 */
function commerce_option_set_form($form, &$form_state, $option_set, $op = 'edit') {
  if ($op == 'clone') {
    $option_set->name .= ' (cloned)';
    $option_set->set_id = '';
  }

  $form['data']['#tree'] = TRUE;

  // Human-readable name.
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($option_set->name) ? $option_set->name : '',
    '#description' => t('The human-readable name of this option set.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable name.
  $form['set_id'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($option_set->set_id) ? $option_set->set_id : '',
    '#maxlength' => 32,
    '#disabled' => isset($option_set->set_id) ? TRUE : FALSE,
    '#machine_name' => array(
      'exists' => 'commerce_option_get_sets',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this option set. It must only contain lowercase letters, numbers, and underscores.'),
  );

  // Description.
  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($option_set->description) ? $option_set->description : '',
    '#title' => t('Description'),
    '#description' => t('This description is shown to the admin user to indicate if this option set is useful for the current purpose.'),
  );

  // Actions.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save option set'),
      '#weight' => 40,
    ),
  );

  if ($op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete option set'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('commerce_option_set_form_submit_delete')
    );
  }

  return $form;
}

/**
 * Form Save submit handler for the option set.
 */
function commerce_option_set_form_submit($form, &$form_state) {
  $option_set = entity_ui_form_submit_build_entity($form, $form_state);

  entity_save('commerce_option_set', $option_set);

  drupal_set_message(t('Option set saved.'));

  // Redirect based on the button clicked.
  if ($form_state['clicked_button']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/products/option-sets/' . strtr($option_set['set_id'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/products/option-sets';
  }
}

/**
 * Delete submit handler.
 *
 * Redirect to page that we defined in hook_menu().
 */
function commerce_option_set_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/products/option-sets/' . $form_state['commerce_option_set']->set_id . '/delete';
}

/**
 * Form callback: confirmation form for deleting a option set.
 * Called as page argument in the page we redirect to in
 * commerce_option_set_form_submit_delete().
 */
function commerce_option_set_delete($form, &$form_state) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';

  $option_set = current(entity_load('commerce_option_set', array(arg(4))));
  $form_state['commerce_option_set'] = $option_set;

  return confirm_form($form,
    t('Are you sure you want to delete the %name option set?', array('%name' => $option_set->name)),
    'admin/commerce/config/option-sets',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
}

/**
 * Submit callback for commerce_option_set_delete_form().
 *
 * @todo: Don't allow deletion of option sets that have options defined?
 */
function commerce_option_set_delete_submit($form, &$form_state) {
  $option_set = $form_state['commerce_option_set'];

  entity_delete('commerce_option_set',  $option_set->set_id);

  // Notify the field API that this bundle has been destroyed.
  field_attach_delete_bundle('commerce_option', $option_set->set_id);

  drupal_set_message(t('The option set %name has been deleted.', array('%name' => $option_set->name)));
  watchdog('commerce_option', 'Deleted option set %name.', array('%name' => $option_set->name), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/products/option-sets';
}
