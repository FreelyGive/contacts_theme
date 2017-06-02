<?php

/**
 * @file
 * Form alters for Contacts Theme.
 */

use Drupal\Core\Form\FormStateInterface;

/**
 * Implements hook_form_FORM_ID_alter().
 */
function contacts_theme_form_views_exposed_form_alter(&$form, FormStateInterface $form_state, $form_id) {
  if ($form_state->get('view')->id() == 'contacts_dashboard_indexed') {
    $form['#attributes']['class'][] = 'contacts-search-form';
    $form['search']['#title_display'] = 'invisible';
    $form['search']['#attributes']['placeholder'] = $form['#info']['filter-search_api_fulltext']['label'];
    $form['actions']['reset']['#attributes']['class'][] = 'link';
  }
}


function contacts_theme_form_profile_form_alter(&$form, FormStateInterface $form_state) {
  if ($form_state->getFormObject()->getOperation() == 'crm_dashboard') {
    $form['#form_columns'] = TRUE;
  }
}

function contacts_theme_form_user_form_alter(&$form, FormStateInterface $form_state) {
  if ($form_state->getFormObject()->getOperation() == 'default') {
    $form['#form_columns'] = ['description' => FALSE];
  }
}
