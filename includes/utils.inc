<?php

/**
 * @file
 * Utility functions for Contacts Theme.
 */

/**
 * Get/set the header image.
 *
 * @param array|NULL $content
 *   The render array for the header image if setting.
 *
 * @return array|NULL
 *   The render array for the header image or NULL if there is none.
 */
function contacts_theme_header_image(array $content = NULL) {
  $image = &drupal_static('contacts_theme_header_image');
  if ($content) {
    $image = $content;
  }
  return $image;
}
