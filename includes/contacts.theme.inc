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

  // Add the manage sidebar.
  if (!empty($variables['content']['#page_variant']) && $variables['content']['#page_variant']->getOriginalId() == 'contacts_dashboard_contact-panels_variant-0') {
    $variables['content']['manage'] = [
      '#theme' => 'contacts_dash_manage',
      '#content' => contacts_theme_dashboard_manage_sidebar_content(),
      '#attached' => ['library' => 'contacts_theme/drag_n_drop'],
    ];

    $route_matcher = \Drupal::routeMatch();
    $user = $route_matcher->getParameter('user');
    if ($user) {
      $variables['content']['manage']['#user'] = $user->id();
    }

    $subpage = $route_matcher->getParameter('subpage');
    if ($subpage) {
      $variables['content']['manage']['#subpage'] = $subpage;
    }
  }
}

/**
 * Builds a render array for the dashboard management sidebar.
 */
function contacts_theme_dashboard_manage_sidebar_content() {
  $content = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['sidebar-manage-content'],
    ],
  ];

  $content['message'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => 'Drag and Drop elements to the page.',
  ];
  $content['types'] = [
    '#type' => 'container'
  ];
  $content['types']['profiles'] = [
    '#type' => 'container'
  ];
  $content['types']['profiles']['title'] = [
    '#type' => 'html_tag',
    '#tag' => 'h5',
    '#value' => 'Profiles',
    '#attributes' => [
      'class' => ['facet-toggle', 'collapsed'],
      'data-target' => "#facet-dropdown-button-profile",
      'data-toggle' => 'collapse',
      'aria-controls' => 'facet-dropdown-button',
      'aria-expanded' => FALSE,
    ],
  ];
  $content['types']['profiles']['content'] = [
    '#type' => 'container',
    '#attributes' => [
      'id' => "facet-dropdown-button-profile",
      'aria-expanded' => FALSE,
      'class' => ['collapse'],
    ],
  ];

  /* @var Drupal\Core\Routing\CurrentRouteMatch $route_match */
  $route_match = \Drupal::getContainer()->get('current_route_match');
  $user = $route_match->getRawParameter('user');
  $subpage = $route_match->getRawParameter('subpage');

  if ($user && $subpage) {
    /* @var Drupal\Core\Entity\EntityTypeManager $entity_type_manager */
    $entity_type_manager = \Drupal::getContainer()->get('entity_type.manager');
    /* @var Drupal\contacts\ContactsTabManager $tab_manager */
    $tab_manager = \Drupal::getContainer()->get('contacts.tab_manager');

    /* @var Drupal\user\UserInterface $user */
    $user = $entity_type_manager->getStorage('user')->load($user);
    $tab = $tab_manager->getTabByPath($user, $subpage);

    $contexts = [];
    foreach ($tab->getRelationships() as $relationship) {
      $parts = explode(':', $relationship['id']);

      if ($parts[0] == 'typed_data_entity_relationship') {
        $contexts[$relationship['name']] = $parts[3];
      }
    }

    $placed_blocks = [];
    foreach ($tab->getBlocks() as $key => $placed_block) {
      if ($placed_block['id'] == 'contacts_entity:profile' && !empty($placed_block['context_mapping']['entity'])) {
        $placed_blocks[$key] = $contexts[$placed_block['context_mapping']['entity']];
      }
    }

    $types = $entity_type_manager->getStorage('profile_type')->loadMultiple();
    foreach ($types as $id => $type) {
      // Skip if missing relationship.
      if (!in_array('profile_' . $id, $contexts)) {
        continue;
      }

      // Skip if already placed.
      if (in_array('profile_' . $id, $placed_blocks)) {
        continue;
      }

      $key = substr($id, 4);
      $content['types']['profiles']['content'][$id] = [
        '#theme' => 'contacts_dnd_card',
        '#key' => $key,
        '#entity' => 'profile',
        '#bundle' => $id,
        '#label' => $type->label(),
        '#description' => "Profile for $key",
        '#link' => [
          'text' => 'Manage Fields',
          'url' => "/admin/config/people/profiles/types/manage/$id/fields",
        ],
      ];
    }
  }

  $content['toggle'] = [
    '#type' => 'html_tag',
    '#tag' => 'button',
    '#value' => 'Exit Manage Mode',
    '#attributes' => [
      'class' => ['trigger-manage', 'btn', 'btn-primary', 'btn-block'],
      'data-target' => '.layout-sidebar-second',
    ],
  ];

  return $content;
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
