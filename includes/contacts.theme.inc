<?php

/**
 * @file
 * Contacts module related theme functions and hooks for Contacts Theme.
 */

use Drupal\Core\Render\Element;

/**
 * Implements hook_preprocess_HOOK() for contacts_dashboard layout.
 */
function contacts_theme_preprocess_contacts_dashboard(array &$variables) {
  $in_dashboard_page = &drupal_static('contacts_theme_dashboard');
  $in_dashboard_page = TRUE;

  foreach (Element::children($variables['content']) as $region) {
    unset($variables['content'][$region]['#prefix']);
    unset($variables['content'][$region]['#suffix']);
  }
}

/**
 * Implements hook_preprocess_HOOK() for contacts_dash_summary.
 */
function contacts_theme_preprocess_contacts_dash_summary(array $variables) {
  contacts_theme_header_image($variables['image']);
}

/**
 * Implements hook_preprocess_HOOK() for contacts_dash_tabs.
 */
function contacts_theme_preprocess_contacts_dash_tabs(&$variables) {
  $variables['attributes']['class'][] = 'nav';
  $variables['attributes']['class'][] = 'nav-tabs';
  foreach ($variables['tabs'] as &$tab) {
    $tab['attributes']->addClass('nav-item');
    $tab['link_attributes']->addClass('nav-link');
    if ($tab['attributes']->hasClass('is-active')) {
      $tab['attributes']->removeClass('is-active');
      $tab['link_attributes']->addClass('active');
      $tab['link_attributes']->removeClass('is-active');
    }
  }
}
