<?php

/**
 * @file
 * Contacts module related theme functions and hooks for Contacts Theme.
 */

use Drupal\Core\Render\Element;
use Drupal\Core\Template\Attribute;
use Drupal\Core\Entity\EntityTypeManager;

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
    $sidebar = contacts_theme_dashboard_manage_sidebar_content();
    $variables['content']['manage'] = [
      '#theme' => 'contacts_dash_manage',
      '#content' => $sidebar['content'],
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
  /* @var Drupal\Core\Routing\CurrentRouteMatch $route_match */
  $route_match = \Drupal::getContainer()->get('current_route_match');
  $user = $route_match->getRawParameter('user');
  $subpage = $route_match->getRawParameter('subpage');

  /* @var Drupal\Core\Entity\EntityTypeManager $entity_type_manager */
  $entity_type_manager = \Drupal::getContainer()->get('entity_type.manager');
  /* @var Drupal\contacts\ContactsTabManager $tab_manager */
  $tab_manager = \Drupal::getContainer()->get('contacts.tab_manager');

  /* @var Drupal\user\UserInterface $user */
  $user = $entity_type_manager->getStorage('user')->load($user);
  $tab = $tab_manager->getTabByPath($user, $subpage);

  $content = [];
  $classes = [
    'blocks' => 'use-ajax p-4 nav-link',
    'views' => 'use-ajax p-4 nav-link',
    'custom' => 'use-ajax p-4 nav-link',
  ];
  $manage_mode_tab = \Drupal::state()->get('manage_mode_tab');
  if (isset($classes[$manage_mode_tab])) {
    $classes[$manage_mode_tab] .= ' active';
  }

  $content['nav'] = [
    '#markup' => '<ul class="nav nav-tabs"><li class="nav-item">
        <a data-ajax-url="/admin/contacts-ajax/manage-mode-tab/' . $user->id() . '/' . $subpage . '/blocks" data-ajax-progress="fullscreen" href="#" title="Manage" class="' . $classes['blocks'] . '"><i class="fa fa-th-large fa-2x" aria-hidden="true"></i></a>
      </li><li class="nav-item">
        <a data-ajax-url="/admin/contacts-ajax/manage-mode-tab/' . $user->id() . '/' . $subpage . '/views" data-ajax-progress="fullscreen" href="#" title="Manage" class="' . $classes['views'] . '"><i class="fa fa-table fa-2x" aria-hidden="true"></i></a>
      </li><li class="nav-item">
        <a data-ajax-url="/admin/contacts-ajax/manage-mode-tab/' . $user->id() . '/' . $subpage . '/custom" data-ajax-progress="fullscreen" href="#" title="Manage" class="' . $classes['custom'] . '"><i class="fa fa-code fa-2x" aria-hidden="true"></i></a>
      </li></ul>',
  ];

  $content['manage_toggle'] = [
    '#type' => 'html_tag',
    '#tag' => 'a',
    '#attributes' => [
      'data-ajax-url' => "/admin/contacts-ajax/manage-mode/{$user->id()}/{$subpage}",
      'data-ajax-progress' => 'fullscreen',
      'href' => '#',
      'title' => 'Manage',
      'class' => ['use-ajax', 'dnd-manage', 'trigger-manage', 'btn', 'btn-lg', 'btn-primary'],
      'data-target' => '.layout-sidebar-second',
    ],
    '#value' => '',
  ];

  switch ($manage_mode_tab) {
    case 'views':
      $content += contacts_theme_dashboard_manage_sidebar_content_views();
      break;
    case 'custom':
      $content += contacts_theme_dashboard_manage_sidebar_content_custom();
      break;
    default:
      $content += contacts_theme_dashboard_manage_sidebar_content_profiles($tab, $user, $entity_type_manager);
      break;
  }

  $content['content']['toggle'] = [
    '#type' => 'html_tag',
    '#tag' => 'button',
    '#value' => 'Exit Manage Mode',
    '#attributes' => [
      'data-ajax-url' => "/admin/contacts-ajax/manage-mode/{$user->id()}/{$subpage}",
      'data-ajax-progress' => 'fullscreen',
      'class' => ['use-ajax', 'trigger-manage', 'btn', 'btn-primary', 'btn-block'],
      'data-target' => '.layout-sidebar-second',
    ],
  ];

  return ['content' => $content];
}

/**
 * Builds a list of draggable blocks for the manage sidebar.
 *
 * @param \Drupal\contacts\Entity\ContactTab $tab
 *   Currently active tab.
 * @param \Drupal\user\Entity\User $user
 *   Currently viewed user.
 * @param \Drupal\Core\Entity\EntityTypeManager $entity_type_manager
 *   The entity type manager service.
 *
 * @return array
 *   Drupal renderable array for draggable block markup.
 */
function contacts_theme_dashboard_manage_sidebar_content_profiles($tab, $user, EntityTypeManager $entity_type_manager) {
  $content['title'] = [
    '#type' => 'html_tag',
    '#tag' => 'h4',
    '#value' => 'Profiles',
    '#attributes' => ['class' => ['text-center', 'card-header']]
  ];

  $content['content'] = [
    '#type' => 'container',
    '#attributes' => ['class' => ['m-4']]
  ];

  $content['content']['message'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => 'Drag and Drop blocks to the page.',
  ];

  $content['content']['types'] = [
    '#type' => 'container'
  ];
  $content['content']['types']['profiles'] = [
    '#type' => 'container',
    '#attributes' => ['class' => 'h100'],
  ];

  $content['content']['types']['profiles']['content'] = [
    '#type' => 'container',
  ];

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

  $definitions = \Drupal::service('plugin.manager.block')->getDefinitions();

  foreach ($definitions as $key => $definition) {
    if (empty($definition['dashboard_block'])) {
      unset($definitions[$key]);
    }
  }

  foreach ($definitions as $id => $definition) {
    // Skip if already placed.
    if (in_array('profile_' . $id, $placed_blocks)) {
      continue;
    }

    if (!empty($definition['_required_hats'])) {
      if (empty(array_intersect($definition['_required_hats'], $user->getRoles()))) {
        continue;
      }
    }

    $content['content']['types']['profiles']['content'][$id] = [
      '#theme' => 'contacts_dnd_card',
      '#attributes' => [
        'class' => ['draggable', 'card'],
      ],
      '#id' => $id,
      '#user' => $user->id(),
      '#mode' => 'meta',
    ];
  }

  return $content;
}

/**
 * Builds a list of draggable views blocks for the manage sidebar.
 */
function contacts_theme_dashboard_manage_sidebar_content_views() {
  $content['title'] = [
    '#type' => 'html_tag',
    '#tag' => 'h4',
    '#value' => 'Views',
    '#attributes' => ['class' => ['text-center', 'card-header']]
  ];

  $content['content'] = [
    '#type' => 'container',
    '#attributes' => ['class' => ['m-4']]
  ];
  $content['content']['message'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => 'Drag and Drop blocks to the page.',
  ];

  return $content;
}

/**
 * Builds a list of custom draggable blocks for the manage sidebar.
 */
function contacts_theme_dashboard_manage_sidebar_content_custom() {
  $content['title'] = [
    '#type' => 'html_tag',
    '#tag' => 'h4',
    '#value' => 'Custom content',
    '#attributes' => ['class' => ['text-center', 'card-header']]
  ];

  $content['content'] = [
    '#type' => 'container',
    '#attributes' => ['class' => ['m-4']]
  ];

  $content['content']['message'] = [
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => 'Create custom content blocks.',
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
