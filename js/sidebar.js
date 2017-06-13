/**
 * @file
 * Contacts' enhancements to actions to allow dropdown grouping.
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
            $('.action-group-control', context).on('click', function () {
                var $this = $(this);
                $('#' + $this.attr('data-action-group')).toggle();
            });

            $(".action-group-drag-n-drop").click(function(e) {
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
