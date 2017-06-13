/**
 * @file
 * Contacts' theme enhancements to actions to allow drag and drop.
 */

(function ($, Drupal) {

    'use strict';

    Drupal.behaviors.contactsThemeDraggable = {
        attach: function (context, settings) {
            $(document).on('dragActive', function() {
                $('.layout-sidebar-second .draggable').draggable({
                    revert: "invalid",
                    helper: "clone",
                    connectToSortable: ".drag-area",
                    stop: function( event, ui ) {
                        ui.helper.removeAttr('style');
                        ui.helper.addClass('draggable-active');
                        ui.helper.addClass('card');
                        ui.helper.find('h2').addClass('card-header');
                    }
                });
            });
        }
    };

    Drupal.behaviors.contactsThemeDragMeta = {
        attach: function (context, settings) {


            function addEdit() {
                $('.edit-draggable').each(function () {
                    var editLink = $(this);
                    if (!editLink.hasClass('processed')) {
                        editLink.addClass('processed');
                        editLink.click(function (ev) {
                            ev.preventDefault();
                            var heading = $(this).parent().parent().prev(),
                                fields = '<input type="text" name="firstname" value="'+heading.text()+'" class="js-text-full text-full form-textfield form-control">' +
                                        '<input type="submit" value="Save" class="js-form-submit drag-save-title form-submit btn btn-primary">';
                            heading.replaceWith(fields);

                            $('.drag-save-title').click(function() {
                                var heading = $(this).siblings("[name='firstname']"),
                                    newHeading = heading.val();
                                heading.replaceWith('<h2 class="card-header">'+newHeading+'</h2>');
                                $(this).remove();
                            });
                        });
                    }
                });
            }

            function addDelete() {
                $('.delete-draggable').each(function () {
                    var deleteLink = $(this);
                    if (!deleteLink.hasClass('processed')) {
                        deleteLink.addClass('processed');
                        deleteLink.click(function (ev) {
                            ev.preventDefault();
                            $(this).parents('.draggable').remove();
                        });
                    }
                });
            }

            $('.draggable h2').each(function(i) {
                var markup = $('<div class="drag-meta card-block">' +
                    '<a class="card-link edit-draggable" href="#">Edit</a>' +
                    '<a class="card-link delete-draggable" href="#">Delete</a>' +
                    '</div>');
                $(this).next().prepend(markup);
                addEdit();
                addDelete();
            })
        }
    };

    Drupal.behaviors.contactsThemeSortable = {
        attach: function (context, settings) {
            $(document).on('dragActive', function() {
                var dragArea = $('.drag-area'),
                    draggable = $('.drag-area .draggable'),
                    draggableHeading = $('.drag-area .draggable h2');
                dragArea.sortable({
                    revert: true,
                    placeholder: "drag-area-placeholder",
                    connectWith: ".drag-area",
                    over: function( event, ui ) {
                        $('.drag-area.highlighted').removeClass('highlighted');
                        $(ui.placeholder).parents('.drag-area').addClass('highlighted');
                    },
                    stop: function( event, ui ) {
                        $('.drag-area.highlighted').removeClass('highlighted');
                    }
                });
                dragArea.sortable('enable');
                dragArea.addClass('show');
                draggable.addClass('draggable-active');
                draggable.addClass('card');
                draggableHeading.addClass('card-header');
                draggableHeading.each(function() {
                    var text = $(this).children('a').text(),
                        i = text.indexOf('Edit');
                    if (i >= 0) {
                        $(this).children('a').remove();
                    }
                });

            });

            $(document).on('dragInactive', function() {
                var dragArea = $('.drag-area'),
                    draggable = $('.drag-area .draggable'),
                draggableHeading = $('.drag-area .draggable h2');
                dragArea.sortable('disable');
                dragArea.removeClass('show');
                draggable.removeClass('draggable-active');
                draggable.removeClass('card');
                draggableHeading.removeClass('card-header');
            });
        }
    };

})(jQuery, Drupal);
