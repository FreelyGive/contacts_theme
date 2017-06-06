<?php

/**
 * @file
 * Page related theme functions and hooks for Contacts Theme.
 */

use Drupal\Core\Template\Attribute;

/**
 * Implements hook_preprocess_HOOK() for page.
 *
 * @todo: Replace with an SVG logo.
 */
function contacts_theme_preprocess_page(array &$variables) {
  $path = drupal_get_path('theme', 'contacts_theme');
  $logo = theme_get_setting('logo.url');
  if ($logo == file_url_transform_relative(file_create_url($path . '/logo.svg'))) {
    $variables['logo'] = file_url_transform_relative(file_create_url($path . '/logo.png'));
  }

  // See if we have a header image.
  $variables['header_image'] = contacts_theme_header_image();
  if ($variables['header_image']) {
    $variables['attributes']['class'][] = 'with-header-image';
  }
}

/**
 * Implements hook_preprocess_HOOK() for status_messages.
 *
 * Add bootstrap classes for alerts.
 */
function contacts_theme_preprocess_status_messages(&$variables) {
  $variables['type_classes'] = [
    'error' => 'danger',
    'warning' => 'warning',
    'status' => 'info',
  ];
}

/**
 * Implement hook_preprocess_HOOK() for menu_local_tasks.
 */
function contacts_theme_preprocess_menu_local_task(&$variables) {
  $variables['attributes']['class'][] = 'nav-item';
  $variables['link']['#options']['attributes']['class'][] = 'nav-link';
  $variables['link']['#options']['set_active_class'] = FALSE;
  if (!empty($variables['is_active'])) {
    $variables['link']['#options']['attributes']['class'][] = 'active';
  }
}

/**
 * Implements hook_preprocess_HOOK() for pager.
 *
 * @see \template_preprocess_pager()
 */
function contacts_theme_preprocess_pager(&$variables) {
  if (isset($variables['items'])) {
    foreach ($variables['items'] as $key => &$item) {
      if ($key == 'pages') {
        foreach ($item as &$page_item) {
          $page_item['attributes'] = new Attribute(['class' => ['page-link']]);
        }
      }
      else {
        $item['attributes'] = new Attribute(['class' => ['page-link']]);
      }
    }
  }
}
