<?php

/**
 * @file
 * Views related hooks for Contacts Theme.
 */

use Drupal\views\ViewExecutable;
use Drupal\Component\Utility\Html;

/**
 * Implements hook_views_pre_render().
 */
function contacts_theme_views_pre_render(ViewExecutable $view) {
  if ($view->id() == 'contacts_dashboard_indexed') {
    $view->display_handler->setOption('css_class', 'view-fill-scroll');
    if (isset($view->header['result'])) {
      $view->header['result']->options['content'] = '<div class="view-result-header">' . $view->header['result']->options['content'] . '</div>';
    }
  }

  $css_class = Html::getClass('views-view-' . $view->id()) . ' ' . $view->display_handler->getOption('css_class');
  $view->display_handler->setOption('css_class', $css_class);
}
