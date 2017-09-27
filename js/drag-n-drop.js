/**
 * @file
 * Contacts' theme enhancements to actions to allow drag and drop.
 */

(function ($, Drupal) {

    'use strict';

    Drupal.DnDAddEdit = function() {
        return;
        $('.drag-area .edit-draggable').each(function () {
            var editLink = $(this);
            if (!editLink.hasClass('processed')) {
                editLink.addClass('processed');
                editLink.click(function (ev) {
                    ev.preventDefault();
                    var heading = $(this).siblings('input[name="card-title"]'),
                        display = $(this).siblings('select[name="card-display"]'),
                        header = $(this).parent();

                    if (header.hasClass('editing')) {
                        var block = $(this).closest('.draggable'),
                            id = block.attr('data-dnd-contacts-block-id'),
                            tab = block.attr('data-dnd-contacts-block-tab'),
                            postData = {
                                label: heading.val(),
                                display: display.val()
                            },
                            title = heading.val(),
                            url = '/admin/contacts-ajax/update-layout';
                        console.log(url);
                        console.log(postData);
                        console.log(display);
                        $.post(url, postData, function( data ) {
                            console.log(data);
                        });

                        header.removeClass('editing');
                        heading.attr('disabled', true);
                    }
                    else {
                        header.addClass('editing');
                        heading.attr('disabled', false);
                    }
                });
            }
        });
    };

    Drupal.DnDAddDelete = function() {
        $('.drag-area .delete-draggable').each(function () {
            var deleteLink = $(this);
            if (!deleteLink.hasClass('processed')) {
                deleteLink.addClass('processed');
                deleteLink.click(function (ev) {
                    ev.preventDefault();
                    var block = $(this).closest('.draggable'),
                        id = block.attr('data-dnd-contacts-block-id'),
                        tab = block.attr('data-dnd-contacts-block-tab'),
                        url = '/contacts_remove_block/'+tab+'/'+id;
                    console.log(url);
                    $.get(url, function( data ) {
                        console.log(data);
                    });
                    $('.nav-tabs a.active').click();
                });
            }
        });
    };

    Drupal.DnDBuildRegion = function(tab, region, sort) {
        if ($(region).hasClass('dash-left')) {
            region = 'left';
        }
        else {
            region = 'right';
        }
        var data = {
            'tab': tab,
            'region': region,
            'blocks': []
        };
        var arrayLength = sort.length;
        for (var weight = 0; weight < arrayLength; weight++) {
            var el = $('[data-dnd-contacts-block-id='+sort[weight]+']');

            // @todo check that profile type and relationship are available.
            var block_data = {
                id: sort[weight],
                profile_type: el.attr('data-dnd-contacts-profile-type'),
                profile_relationship: el.attr('data-dnd-contacts-profile-relationship')
            };
            console.log(block_data);
            data.blocks.push(block_data);
        }

        return data;
    };

    Drupal.DnDUpdateRegion = function(tab, blocks) {
        var url = '/admin/contacts-ajax/update-layout',
            postData = {
                block_data: blocks,
                tab: tab,
            };
        console.log(url);
        $.post(url, postData, function( data ) {
            $('.nav-tabs a.active').click();
        });
    };

    Drupal.DnDUpdateTitle = function(region, sort) {
        return;
        var arrayLength = sort.length;
        for (var weight = 0; weight < arrayLength; weight++) {
            var id = sort[weight];
            var tab = $('[data-dnd-contacts-block-id='+id+']').attr('data-dnd-contacts-block-tab');
            var url = '/contacts_move_block/'+tab+'/'+id+'/'+region+'/'+weight;
            console.log(url);
            $.get(url, function( data ) {
                console.log(data);
            });
        }
    };


    Drupal.behaviors.contactsThemeDraggable = {
        attach: function (context, settings) {
            $(document).on('dragActive', function() {
                $('.layout-sidebar-second .draggable').draggable({
                    revert: "invalid",
                    // helper: "clone",
                    connectToSortable: ".drag-area",
                    stop: function( event, ui ) {
                        // var tab = $('.nav-tabs a.active').attr('data-contacts-tab-id');
                        // ui.helper.attr('data-dnd-contacts-block-tab', tab);
                        // Drupal.DnDAddBlock(ui.helper);

                        console.log('drag stop');
                    }
                });
            });
        }
    };

    Drupal.behaviors.contactsThemeDragTabs = {
        attach: function (context, settings) {
            $(document).on('dragActive', function() {
                $('.contacts-ajax-tabs').addClass('editing');
                // $('.contacts-ajax-tabs .nav-link').each(function() {
                    // $(this).addClass('working');
                    // $(this).unbind();
                    // $(this).click(function(ev) {
                    //     ev.preventDefault();
                    // });

                // });

                // $('.contacts-ajax-tabs').sortable({
                //     revert: true,
                //     placeholder: "nav-item nav-link tab-area-placeholder",
                //     // connectWith: ".drag-area",
                //     over: function( event, ui ) {
                //         // $('.drag-area.highlighted').removeClass('highlighted');
                //         // $(ui.placeholder).parents('.drag-area').addClass('highlighted');
                //     },
                //     stop: function( event, ui ) {
                //         // $('.drag-area.highlighted').removeClass('highlighted');
                //     }
                // });
            });

            $(document).on('dragInactive', function() {
                $('.contacts-ajax-tabs').removeClass('editing');
            });

            $('.draggable .card-header').each(function(i) {
            //     var markup = $('<a class="card-link edit-draggable" href="#">Edit</a>' +
            //         '<a class="card-link delete-draggable" href="#">Delete</a>');
            //     // $(this).next().prepend(markup);
            //     $(this).append(markup);
                Drupal.DnDAddEdit();
                Drupal.DnDAddDelete();
            })
        }
    };

    Drupal.behaviors.contactsThemeSortable = {
        attach: function (context, settings) {
            function replaceHeader(header) {
                var text = $(header).children('a').text(),
                    i = text.indexOf('Edit');
                if (i >= 0) {
                    $(header).children('a').remove();
                }

                var headerText = $(header).text(),
                    markup = '<form class="form-inline card-header">' +
                        '<input type="text" disabled class="form-control mb-2 mr-sm-2 mb-sm-0" name="card-title" value="'+headerText+'">' +
                        '<label class="mr-sm-2 display-select" for="inlineFormCustomSelect">Display</label>' +
                        '<select class="display-select custom-select mb-2 mr-sm-2 mb-sm-0" name="card-display">' +
                        '<option selected value="1">Default</option>' +
                        '<option value="2">Teaser</option>' +
                        '<option value="3">Dashboard</option>' +
                        '</select>' +
                        '</form>';

                $(header).replaceWith(markup);
            }
            $(document).on('dragActive', function() {
                var dragArea = $('.drag-area'),
                    draggable = $('.drag-area .draggable'),
                    draggableHeading = $('.drag-area .draggable h2');
                dragArea.sortable({
                    revert: true,
                    placeholder: "drag-area-placeholder",
                    connectWith: ".drag-area",
                    items: ".draggable",
                    over: function( event, ui ) {
                        $('.drag-area.highlighted').removeClass('highlighted');
                        $(ui.placeholder).parents('.drag-area').addClass('highlighted');
                    },
                    stop: function( event, ui ) {
                        console.log('sort stop');

                        var blocks = [];
                        var tab = $('.nav-tabs a.active').attr('data-contacts-tab-id');
                        $(".drag-area").each(function() {
                            var sortedIDs = $(this).sortable("toArray", {attribute: 'data-dnd-contacts-block-id'});
                            if (sortedIDs.length) {
                                var data = Drupal.DnDBuildRegion(tab, this, sortedIDs);
                                blocks.push(data);
                            }
                        });

                        Drupal.DnDUpdateRegion(tab, blocks);
                        console.log(tab);
                        console.log(blocks);

                        $('.drag-area.highlighted').removeClass('highlighted');
                    }
                });
                dragArea.sortable('enable');
                // dragArea.addClass('show');
                // draggable.addClass('draggable-active');
                // draggable.addClass('card');

                // draggableHeading.each(function() {
                //     replaceHeader(this);
                // });

                // $('.drag-area .draggable .card-header, .draggable .page-content .card-header').each(function(i) {
                    // if (!$(this).hasClass('links-added')) {
                    //     $(this).addClass('links-added');
                    //     var markup = $('<a class="ml-auto align-self-center card-link edit-draggable" href="#">&nbsp</a>' +
                    //         '<a class="card-link delete-draggable" href="#">&nbsp</a>');
                    //     $(this).append(markup);
                    //     Drupal.DnDAddEdit();
                    //     Drupal.DnDAddDelete();
                    // }
                // })
            });

            $(document).on('dragInactive', function() {
                var dragArea = $('.drag-area'),
                    draggable = $('.drag-area .draggable'),
                    draggableHeading = $('.drag-area .draggable .card-header');
                if (!dragArea.hasClass('ui-sortable')) {
                    return;
                }
                dragArea.sortable('disable');
                // dragArea.removeClass('show');
                // draggable.removeClass('draggable-active');
                // draggable.removeClass('card');
                draggableHeading.removeClass('card-header');
                draggableHeading.each(function() {
                   var headingText = $(this).children('input[name="card-title"]').val(),
                       markup = '<h2>'+headingText+'</h2>';
                    $(this).replaceWith(markup);
                });
            });
        }
    };

    Drupal.behaviors.contactsThemeDnDInit = {
        attach: function (context, settings) {
            var trigger = $(".trigger-manage");
            var sidebar = $(trigger.attr('data-target'));

            if (settings.dragMode) {
                sidebar.removeClass("toggled");
                $(document).trigger('dragActive');
            }
            else {
                sidebar.addClass("toggled");
                $(document).trigger('dragInactive');
            }
        }
    };

})(jQuery, Drupal);
