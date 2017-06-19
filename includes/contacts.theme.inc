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

  /* @var Drupal\Core\Routing\CurrentRouteMatch $route_match */
  $route_match = \Drupal::getContainer()->get('current_route_match');
  $user = $route_match->getRawParameter('user');
  $subpage = $route_match->getRawParameter('subpage');

  if ($user && $subpage) {
    /* @var Drupal\Core\Entity\EntityTypeManager $entity_type_manager */
    $entity_type_manager = \Drupal::getContainer()->get('entity_type.manager');
    /* @var Drupal\contacts\ContactsTabManager $tab_manager */
    $tab_manager = \Drupal::getContainer()->get('contacts.tab_manager');

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
      if ($placed_block['id'] = 'contacts_entity:profile' && !empty($placed_block['context_mapping']['entity'])) {
        $placed_blocks[$key] = $contexts[$placed_block['context_mapping']['entity']];
      }
    }

    $variables['content']['manage'] = [
      '#theme' => 'contacts_dash_manage',
      '#types' => ['profile' => [
        'label' => 'Profiles',
        'items' => [],
      ]],
      '#attributes' => ['id' => 'collapseExample'],
    ];

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

      /* @var \Drupal\profile\Entity\ProfileType $type */
      $variables['content']['manage']['#types']['profile']['items'][$id] = [
        'id' => $id,
        'label' => $type->label(),
        'type' => substr($id, 4),
      ];
    }
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
