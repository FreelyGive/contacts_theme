<?php

/**
 * @file
 * Contacts module related theme functions and hooks for Contacts Theme.
 */

use Drupal\Component\Utility\Html;

/**
 * Implements hook_preprocess_HOOK() for block templates.
 */
function contacts_theme_preprocess_block(&$variables) {
  // Add classes to specific blocks for styling.
  $classes = [
    'views_exposed_filter_block:contacts_dashboard_indexed-full' => ['highlight'],
    'contacts_back' => ['highlight'],
  ];
  if (isset($classes[$variables['plugin_id']])) {
    if (!isset($variables['attributes']['class'])) {
      $variables['attributes']['class'] = $classes[$variables['plugin_id']];
    }
    else {
      $variables['attributes']['class'] = array_merge($variables['attributes']['class'], $classes[$variables['plugin_id']]);
    }
  }

  if ($variables['base_plugin_id'] == 'facet_block') {
    $variables['#attached']['library'][] = 'contacts_theme/bootstrap.collapse';

    $collapsed = !in_array($variables['plugin_id'], array('facet_block:crm_roles', 'facet_block:crm_type'));
    $facet_id = Html::getUniqueId('facet-dropdown-button');

    // Add some margin between facets.
    $variables['attributes']['class'][] = 'mb-4';

    // Add properties to facet block title.
    $variables['title_attributes']['class'][] = 'facet-toggle';
    $variables['title_attributes']['data-target'][] = "#{$facet_id}";
    $variables['title_attributes']['data-toggle'][] = 'collapse';
    $variables['title_attributes']['aria-controls'][] = $facet_id;

    // Add properties to facet block content wrapper.
    $variables['content_attributes']['id'] = $facet_id;
    $variables['content_attributes']['class'][] = 'collapse';

    // Set the default collapsed state.
    if ($collapsed) {
      $variables['title_attributes']['aria-expanded'][] = 'false';
      $variables['title_attributes']['class'][] = 'collapsed';
      $variables['content_attributes']['aria-expanded'][] = 'false';
    }
    else {
      $variables['title_attributes']['aria-expanded'][] = 'true';
      $variables['content_attributes']['aria-expanded'][] = 'true';
      $variables['content_attributes']['class'][] = 'show';

    }

    // Add our facet classes.
    $content = &$variables['content'][0];
    foreach ($content['#items'] as &$item) {
      $item['#attributes']['class'][] = 'facet-item';
      $item['#attributes']['class'][] = 'facet-item-default';
      $item['#attributes']['class'][] = 'd-flex';

      // Add active class, if required.
      if (in_array('is-active', $item['#attributes']['class'])) {
        $item['#attributes']['class'][] = 'active';
      }
    }
  }
  elseif ($variables['plugin_id'] == 'local_actions_block') {
    $keys = [
      'contacts.contact_create_group',
      'contacts.contact_configure_group',
    ];
    foreach ($variables['content'] as $key => &$content) {
      if (in_array($key, $keys)) {
        $content['#attributes']['class'] = array('dropdown');
        $content['#prefix'] = '<div class="action-group-wrapper dropdown">';
        $content['#suffix'] = '</div>';
        $drop_id = Html::getUniqueId('dropdown-menu-button');
        $content['button']['#attributes']['id'][] = $drop_id;
        $content['button']['#attributes']['data-toggle'][] = 'dropdown';
        $content['button']['#attributes']['aria-haspopup'][] = 'true';
        $content['button']['#attributes']['aria-expanded'][] = 'false';
        $content['button']['#attributes']['type'][] = 'button';
        $content['button']['#attributes']['class'][] = 'btn';
        $content['button']['#attributes']['class'][] = 'btn-lg';
        $content['button']['#attributes']['class'][] = 'btn-primary';

        $js_hide = array_search('js-hide', $content['group']['#attributes']['class']);
        if ($js_hide) {
          unset($content['group']['#attributes']['class'][$js_hide]);
        }

        $content['group']['#attributes']['class'][] = 'dropdown-menu';
        $content['group']['#attributes']['class'][] = 'dropdown-menu-right';
        $content['group']['#attributes']['aria-labelledby'][] = $drop_id;

        $links = array();
        foreach ($content['group']['links']['#links'] as $key => &$link) {
          $link['attributes']['class'][] = 'dropdown-item';
          $links[$key] = array(
            '#type' => 'link',
            '#attributes' => $link['attributes'],
            '#title' => $link['title'],
            '#url' => $link['url'],
          );
        }

        // Move links out of unordered list #links.
        if (!empty($links)) {
          $content['group']['links'] = $links;
        }
      }
    }

    // Replace text with icon.
    if (isset($variables['content']['contacts.contact_create_group'])) {
      $variables['content']['contacts.contact_create_group']['button']['#value'] = '+';
      $variables['content']['contacts.contact_create_group']['button']['#attributes']['class'][] = 'button-action-icon';
    }

  }
  elseif ($variables['base_plugin_id'] == 'views_block') {
    if ($variables['elements']['#base_plugin_id'] == 'views_block') {
      $view_classes = explode(' ', $variables['elements']['content']['#view']->display_handler->getOption('css_class'));
      if (in_array('view-fill-scroll', $view_classes)) {
        $variables['attributes']['class'][] = 'app-flex';
        $variables['attributes']['class'][] = 'flex-fill-scroll';
        unset($variables['content']['#theme_wrappers']);
      }
    }
  }
  elseif ($variables['plugin_id'] == 'contacts_back') {
    $pos = array_search('button', $variables['content']['#options']['attributes']['class']);
    if ($pos !== FALSE) {
      unset($variables['content']['#options']['attributes']['class'][$pos]);
    }
    $variables['content']['#options']['attributes']['class'][] = 'btn';
    $variables['content']['#options']['attributes']['class'][] = 'btn-primary';
  }
  elseif ($variables['base_plugin_id'] == 'contacts_entity') {
    // Show inline dashboard fields in columns.
    if (isset($variables['content']['view'])) {
      $variables['content']['view'] += ['#columns' => TRUE];
    }
  }
  elseif ($variables['plugin_id'] == 'contact_summary_tab') {
    $variables['content']['#content']['left']['content']['view']['#columns'] =
    $variables['content']['#content']['right']['content']['view']['#columns'] = [
      'label' => ['col-md-6', 'col-xl-4'],
      'content' => ['col-md'],
      '_offset' => ['offset-md-6', 'offset-xl-4'],
    ];
  }
}
