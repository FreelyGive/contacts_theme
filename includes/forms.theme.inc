<?php

/**
 * @file
 * Form related theme functions and hooks for Contacts Theme.
 */

use Drupal\Component\Render\FormattableMarkup;
use Drupal\Core\Render\Element;

/**
 * Pre render callback to propagate form column settings.
 *
 * @param array $element
 *   The element array.
 *
 * @return array
 *   The adjusted element.
 */
function contacts_theme_form_propagate_columns(array $element, $column_settings = FALSE) {
  if (!empty($element['#form_columns'])) {
    // Convert non arrays to an empty array.
    if (!is_array($element['#form_columns'])) {
      $element['#form_columns'] = array();
    }

    // Add our default settings.
    if (isset($element['#form_columns']['description']) && !$element['#form_columns']['description']) {
      $element['#form_columns']['description'] = ['col-sm-8', 'col-xl-6', 'offset-sm-4', 'offset-xl-2'];
    }
    $element['#form_columns'] += array(
      'label' => ['col-sm-4', 'col-xl-2'],
      'input' => ['col-sm-8', 'col-xl-6'],
      'description' => ['col-sm-12', 'col-xl-4'],
    );
  }
  elseif ($column_settings) {
    $element['#form_columns'] = $column_settings;
  }
  else {
    return $element;
  }

  // Propagate to our children.
  foreach (Element::children($element) as $child) {
    $element[$child] = contacts_theme_form_propagate_columns($element[$child], $element['#form_columns']);
  }

  return $element;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function contacts_theme_preprocess_form_element(&$variables) {
  $variables['element'] += [
    '#form_group' => TRUE,
  ];
  $variables += [
    'use_wrapper' => empty($variables['element']['#no_wrapper']),
    'form_group' => !empty($variables['element']['#form_group']),
    'type' => NULL,
    'wrap_in_label' => isset($variables['type']) && in_array($variables['type'], ['radio', 'checkbox']) && $variables['title_display'] == 'after',
  ];

  if ($variables['use_wrapper']) {
    if ($variables['form_group']) {
      $variables['attributes']['class'][] = 'form-group';
    }

    if (!empty($variables['element']['#form_columns']) && $variables['form_group']) {
      $variables['attributes']['class'][] = 'row';

      if ($variables['title_display'] == 'before') {
        $variables['label']['#attributes'] += ['class' => []];
        $variables['label']['#attributes']['class'] = array_merge($variables['label']['#attributes']['class'], $variables['element']['#form_columns']['label']);

        // If we are not composite fields (radios/checkboxes) use the inline label.
        if (!in_array($variables['type'], ['radios', 'checkboxes'])) {
          $variables['label']['#attributes']['class'][] = 'col-form-label';
        }
      }

      // Add in additional classes if the title isn't displayed before.
      $classes = $variables['element']['#form_columns']['input'];
      if ($variables['title_display'] != 'before') {
        foreach ($variables['element']['#form_columns']['label'] as $class) {
          $classes[] = str_replace('col-', 'offset-', $class);
        }
      }

      // If wrapping, we need to add a title prefix/suffix.
      if ($variables['wrap_in_label']) {
        $variables['label']['#prefix'] = '<div class="' . implode(' ', $classes) . '">';
        $variables['label']['#suffix'] = '</div>';
      }
      // Otherwise wrap the children in a div.
      else {
        $variables['children'] = new FormattableMarkup('<div class="@classes">@children</div>', [
          '@classes' => implode(' ', $classes),
          '@children' => $variables['children'],
        ]);
      }

      if ($variables['description']) {
        $variables['description']['attributes']->addClass($variables['element']['#form_columns']['description']);
      }

    }
  }

  if ($variables['description']) {
    $variables['description']['attributes']->addClass('form-text text-muted');
  }

  // Make radios and checkboxes wrap their label for 'after' display.
  if ($variables['wrap_in_label']) {
    $variables['label']['#attributes']['class'][] = 'form-check-label';
    $variables['label']['#title'] = new FormattableMarkup('@children @title', [
      '@title' => $variables['label']['#title'],
      '@children' => $variables['children'],
    ]);
    $variables['children'] = '';
  }
}

/**
 * Implements hook_preprocess_HOOK().
 */
function contacts_theme_preprocess_textarea(&$variables) {
  $variables['attributes']->addClass('form-control');
}

/**
 * Implements hook_preprocess_HOOK().
 */
function contacts_theme_preprocess_input(&$variables) {
  switch ($variables['element']['#type']) {
    case 'token':
    case 'hidden':
      // Do nothing.
      break;

    case 'submit':
      // If we already have the btn class, assume all other classes are as desired.
      if (!in_array('btn', $variables['attributes']['class'])) {
        $variables['attributes']['class'][] = 'btn';
        $variables['attributes']['class'] = array_diff($variables['attributes']['class'], ['button']);

        // If we have the 'button--primary' class, add our primary.
        if (in_array('button--primary', $variables['attributes']['class'])) {
          $variables['attributes']['class'][] = 'btn-primary';
          $variables['attributes']['class'] = array_diff($variables['attributes']['class'], ['button--primary']);
        }
        // Otherwise assume it's secondary.
        else {
          $variables['attributes']['class'][] = 'btn-secondary';
        }
      }
      break;

    case 'checkbox':
    case 'radio':
      $variables['attributes']['class'][] = $variables['element']['#title_display'] == 'after' ? 'form-check-input' : 'form-check';
      break;

    case 'file':
      $variables['attributes']['class'][] = 'form-control-file';
      break;

    default:
      $variables['attributes']['class'][] = 'form-control';
      break;
  }
}

/**
 * Pre render callback for the actions element.
 */
function contacts_theme_pre_render_actions(&$element) {
  // Align actions with the main column.
  if (!empty($element['#form_columns'])) {
    $element['#prefix'] = '<div class="row">';
    $element['#suffix'] = '</div>';

    $classes = $element['#form_columns']['input'];
    foreach ($element['#form_columns']['label'] as $class) {
      $classes[] = str_replace('col-', 'offset-', $class);
    }

    $element['#attributes'] += ['class' => []];
    $element['#attributes']['class'] = array_merge($element['#attributes']['class'], $classes);
  }

  return $element;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function contacts_theme_preprocess_select(&$variables) {
  $variables['attributes']['class'][] = 'form-control';
  $variables['attributes']['class'][] = 'custom-select';
}

/**
 * Implements hook_preprocess_HOOK().
 */
function contacts_theme_preprocess_datetime_form(&$variables) {
  $variables['attributes']['class'][] = 'form-inline';
  $variables['content'] = contacts_theme_make_inline($variables['content']);
}

/**
 * Pre-render to remove wrappers on child elements.
 */
function contacts_theme_make_inline($element) {
  foreach (Element::children($element) as $child) {
    if (in_array('form_element', $element[$child]['#theme_wrappers'])) {
      $element[$child]['#no_wrapper'] = TRUE;
      $element[$child]['#attributes']['class'][] = 'mr-sm-2';
      $element[$child]['#attributes']['class'][] = 'mb-2';
      $element[$child]['#attributes']['class'][] = 'mb-sm-0';
    }
  }
  return $element;
}

/**
 * Pre-render to make composite children not form groups..
 */
function contacts_theme_composite_pre_render($element) {
  foreach (Element::children($element) as $child) {
    $element[$child]['#form_group'] = FALSE;
  }
  return $element;
}