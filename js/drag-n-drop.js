/**
 * @file
 * Contacts' theme enhancements to actions to allow drag and drop.
 */

(function ($, Drupal) {

    'use strict';

    Drupal.DnDAddEdit = function() {
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
                            id = block.attr('contacts_block_id'),
                            tab = block.attr('contacts_block_tab'),
                            postData = {
                                label: heading.val(),
                                display: display.val()
                            },
                            title = heading.val(),
                            url = '/contacts_update_block/'+tab+'/'+id;
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
                        id = block.attr('contacts_block_id'),
                        tab = block.attr('contacts_block_tab'),
                        url = '/contacts_remove_block/'+tab+'/'+id;
                    console.log(url);
                    $.get(url, function( data ) {
                        console.log(data);
                    });
                    block.remove();
                });
            }
        });
    };

    Drupal.DnDAddBlock = function(block) {
        console.log('add block');
        var region = block.closest('.drag-area');
        if (region.hasClass('dash-left')) {
            region = 'left';
        }
        else {
            region = 'right';
        }
            var id = block.attr('contacts_block_id'),
                tab = block.attr('contacts_block_tab'),
                url = '/contacts_add_block/'+tab+'/'+id+'/'+region+'/0',
                postData = {
                    profile_type: block.attr('contacts_profile_type'),
                    profile_relationship: block.attr('contacts_profile_relationship')
                };
            console.log(url);
            console.log(postData);
            $.post(url, postData, function( data ) {
                console.log(data);
            });

    };

    Drupal.DnDUpdateRegion = function(region, sort) {
        if ($(region).hasClass('dash-left')) {
            region = 'left';
        }
        else {
            region = 'right';
        }
        var arrayLength = sort.length;
        for (var weight = 0; weight < arrayLength; weight++) {
            var id = sort[weight];
            var tab = $('[contacts_block_id='+id+']').attr('contacts_block_tab');
            var url = '/contacts_move_block/'+tab+'/'+id+'/'+region+'/'+weight;
            console.log(url);
            $.get(url, function( data ) {
                console.log(data);
            });
        }
    };

    Drupal.DnDUpdateTitle = function(region, sort) {
        var arrayLength = sort.length;
        for (var weight = 0; weight < arrayLength; weight++) {
            var id = sort[weight];
            var tab = $('[contacts_block_id='+id+']').attr('contacts_block_tab');
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
                        ui.helper.removeAttr('style');
                        ui.helper.addClass('draggable-active');
                        ui.helper.addClass('card');
                        // @todo fix this... very flimsy.
                        var tab = $('.nav-tabs a.active').attr('contacts_tab_id');
                        console.log(tab);
                        ui.helper.attr('contacts_block_tab', tab);
                        ui.helper.find('h2').addClass('card-header');
                        Drupal.DnDAddBlock(ui.helper);
                        Drupal.DnDAddEdit();
                        Drupal.DnDAddDelete();
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

            // $('.draggable .card-header').each(function(i) {
            //     var markup = $('<a class="card-link edit-draggable" href="#">Edit</a>' +
            //         '<a class="card-link delete-draggable" href="#">Delete</a>');
            //     // $(this).next().prepend(markup);
            //     $(this).append(markup);
            //     Drupal.DnDAddEdit();
            //     Drupal.DnDAddDelete();
            // })
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
                        $(".drag-area").each(function() {
                            var sortedIDs = $(this).sortable("toArray", {attribute: 'contacts_block_id'});
                            if (sortedIDs.length) {
                                Drupal.DnDUpdateRegion(this, sortedIDs);
                            }
                        });

                        $('.drag-area.highlighted').removeClass('highlighted');
                    }
                });
                dragArea.sortable('enable');
                dragArea.addClass('show');
                draggable.addClass('draggable-active');
                draggable.addClass('card');

                draggableHeading.each(function() {
                    replaceHeader(this);
                });

                $('.drag-area .draggable .card-header, .draggable .page-content .card-header').each(function(i) {
                    if (!$(this).hasClass('links-added')) {
                        $(this).addClass('links-added');
                        var markup = $('<a class="ml-auto align-self-center card-link edit-draggable" href="#">&nbsp</a>' +
                            '<a class="card-link delete-draggable" href="#">&nbsp</a>');
                        $(this).append(markup);
                        Drupal.DnDAddEdit();
                        Drupal.DnDAddDelete();
                    }
                })
            });

            $(document).on('dragInactive', function() {
                var dragArea = $('.drag-area'),
                    draggable = $('.drag-area .draggable'),
                    draggableHeading = $('.drag-area .draggable .card-header');
                dragArea.sortable('disable');
                dragArea.removeClass('show');
                draggable.removeClass('draggable-active');
                draggable.removeClass('card');
                draggableHeading.removeClass('card-header');
                draggableHeading.each(function() {
                   var headingText = $(this).children('input[name="card-title"]').val(),
                       markup = '<h2>'+headingText+'</h2>';
                    $(this).replaceWith(markup);
                });
            });
        }
    };

})(jQuery, Drupal);
