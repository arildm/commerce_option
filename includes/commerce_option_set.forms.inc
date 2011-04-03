<?php

function commerce_option_set_form($form, &$form_state, $option_set) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_option') . '/includes/commerce_option.forms.inc';

  // Store the initial option set in the form state.
  $form_state['option_set'] = $option_set;

  $form['option_set'] = array(
    '#tree' => TRUE,
  );

  $form['option_set']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $option_set['name'],
    '#description' => t('The human-readable name of this option set. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
    '#field_suffix' => ' <small id="edit-option-set-name-suffix">' . t('Machine name: @set', array('@set' => $option_set['set_id'])) . '</small>',
  );

  if (empty($option_set['set_id'])) {
    $form['option_set']['set_id'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $option_set['set_id'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_option_set_load',
        'source' => array('option_set', 'name'),
      ),
      '#description' => t('The machine-readable name of this option set. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }

  $form['option_set']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this option set. The text will be displayed by the <em>option set field</em>.'),
    '#default_value' => $option_set['description'],
    '#rows' => 3,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save option set'),
    '#submit' => $submit + array('commerce_option_set_form_submit'),
  );

  if (!empty($form_state['option_set']['set_id'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete option set'),
      '#suffix' => l('Cancel', 'admin/commerce/config/option-sets'),
      '#submit' => $submit + array('commerce_option_set_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l('Cancel', 'admin/commerce/config/option-sets'),
      '#submit' => $submit + array('commerce_option_set_form_submit'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_option_set_form_validate';

  return $form;

}




/**
 * Form submit handler for the option set
 */
function commerce_option_set_form_submit($form, &$form_state) {
  $option_set = $form_state['option_set'];
  $updated = !empty($option_set['set_id']);

  foreach ($form_state['values']['option_set'] as $key => $value) {
    $option_set[$key] = $value;
  }

  // Write the option set to the database.
  $option_set['is_new'] = !$updated;
  commerce_option_set_save($option_set);

  
  drupal_set_message(t('Option set saved.'));

  // Redirect based on the button clicked.
  if ($form_state['clicked_button']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/config/option-sets/' . strtr($option_set['set_id'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/config/option-sets';
  }
}


function commerce_option_set_form_validate($form, &$form_state) {
  $option_set = $form_state['option_set'];

  // If saving a new option set, ensure it has a unique machine name.
  if (empty($option_set['set_id'])) {
    if (!commerce_option_set_exists($form_state['values']['option_set']['set_id'])) {
      form_set_error('option_set][set_id', t('The machine name specified is already in use.'));
    }
  }
}



function commerce_option_set_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/option-sets/' . strtr($form_state['option_set']['set_id'], '_', '-') . '/delete';
}



/**
 * Form callback: confirmation form for deleting a option set.
 *
 * @param $option_set
 *   The option set array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_option_set_delete_form($form, &$form_state, $option_set) {
  $form_state['option_set'] = $option_set;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';

  $form['#submit'][] = 'commerce_option_set_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name option set?', array('%name' => $option_set['name'])),
    'admin/commerce/config/option-sets',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_option_set_delete_form().
 */
function commerce_option_set_delete_form_submit($form, &$form_state) {
  $option_set = $form_state['option_set'];

  commerce_option_set_delete($option_set['set_id']);

  drupal_set_message(t('The option set %name has been deleted.', array('%name' => $option_set['name'])));
  watchdog('commerce_option', 'Deleted option set %name.', array('%name' => $option_set['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'commerce_option_set_delete';
}

















