/**
 * @file
 * Contacts' theme enhancements to allow a collapsible sidebar.
 */

(function ($, Drupal) {

    'use strict';

    /**
     * Control dropdown action groups.
     *
     * @type {Drupal~behavior}
     *
     * @prop {Drupal~behaviorAttach} attach
     *   Set our tabs up to use AJAX requests.
     */
    Drupal.behaviors.contactsThemeSideBar = {
        attach: function (context, settings) {
            $(".action-group-drag-n-drop").click(function(e) {
                console.log('click');
                e.preventDefault();
                var sidebar = jQuery(this).attr('data-target');
                sidebar = $(sidebar);
                sidebar.toggleClass("toggled");
                if (sidebar.hasClass('toggled')) {
                    jQuery(document).trigger('dragInactive');
                }
                else {
                    jQuery(document).trigger('dragActive');
                }
            });
        }
    };

})(jQuery, Drupal);
