<?php

/**
 * @file
 * Contacts module related theme functions and hooks for Contacts Theme.
 */

use Drupal\Core\Render\Element;
use Drupal\Core\Template\Attribute;

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

  $variables['content_attributes'] = new Attribute();
  $variables['sidebar_first_attributes'] = new Attribute();
  if ($variables['content']['#page_variant']->getPage()->id() == 'contacts_dashboard') {
    $variables['sidebar_first_attributes']->addClass('flex-first');
  }

  $variables['#attached']['library'][] = 'contacts_theme/drag_n_drop';
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

/**
 * Pre-render callback for contact_tab_content manage links.
 */
function contacts_theme_pre_render_contact_tab_manage_links($element) {
  if ($element['content']) {
    foreach (Element::children($element['content']) as $region) {
      if (!empty($element['content'][$region][0]['link'])) {
        // Add icon to button.
        $title = [
          '#type' => 'container',
          'icon' => [
            '#type' => 'open_iconic',
            '#size' => '10',
            '#icon' => 'plus',
            '#color' => 'transparent',
            '#attributes' => ['class' => ['mr-2']],
          ],
          'title' => [
            '#markup' => $element['content'][$region][0]['link']['#title']
          ],
        ];

        $element['content'][$region][0]['link']['#title'] = $title;
        $element['content'][$region][0]['link']['#attributes']['class'][] = 'btn';
        $element['content'][$region][0]['link']['#attributes']['class'][] = 'btn-primary';
        $element['content'][$region][0]['link']['#attributes']['class'][] = 'btn-sm';
        $element['content'][$region][0]['link']['#attributes']['class'][] = 'mb-3';
      }
    }
  }

  return $element;
}
