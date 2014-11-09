<?php

/**
 * @file
 * Commerce option rules integration
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_option_rules_event_info() {
  $events = array();

  $events['commerce_option_pre_save'] = array(
    'label' => t('Before saving a commerce option'),
    'group' => t('Commerce Option'),
    'variables' => array(
      'commerce_option' => array(
        'label' => t('Commerce option'),
        'type' => 'commerce_option',
      ),
      'commerce_product' => array(
        'label' => t('Commerce product'),
        'type' => 'commerce_product',
      ),
    ),
  );

  $events['commerce_option_post_save'] = array(
    'label' => t('After saving a commerce option'),
    'group' => t('Commerce Option'),
    'variables' => array(
      'commerce_option' => array(
        'label' => t('Commerce option'),
        'type' => 'commerce_option',
      ),
      'commerce_product' => array(
        'label' => t('Commerce product'),
        'type' => 'commerce_product',
      ),
    ),
  );

  return $events;
}
