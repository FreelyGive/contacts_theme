/**
 * @file
 * Contact Dashboard ajax navigation.
 */

(function ($, Drupal, debounce) {

  'use strict';

  /**
   * Allow horizontal scrolling indicators, particularly for mobile.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Set our tab scrolling indicators.
   */
  Drupal.behaviors.contactsThemeTabs = {
    attach: function (context, settings) {
      // Bind the resize behaviour that calculates the class.
      $(window).on('resize', context, debounce(Drupal.behaviors.contactsThemeTabs.onResize, 50));

      $('.nav-tabs', context)
        // Add the scroll behaviour.
        .on('scroll', debounce(Drupal.behaviors.contactsThemeTabs.onScroll, 50))
        // Add our scroll handlers.
        .once('contacst_theme.tabs').each(function() {
          $(this).after('<div class="scroll-next"><span>&gt;</span></div>').after('<div class="scroll-prev"><span>&lt;</span></div>');
        });

      // Trigger it manually to start. We build the event ourselves to avoid
      // calling other window resize handlers.
      var event = $.Event('resize');
      event.data = context;
      Drupal.behaviors.contactsThemeTabs.onResize(event);
    },
    onResize: function(event) {
      $('.nav-tabs', event.data).each(function () {
        var $this = $(this).css('overflow', null).removeClass('scrollbar-controls');

        if (this.scrollWidth > this.offsetWidth) {
          var prevHeight = this.offsetHeight;
          $this.css({
            'overflow-x': 'scroll',
            'overflow-y': 'hidden'
          });

          // If the height has changed, a scrollbar is visible.
          if (prevHeight !== this.offsetHeight) {

          }
          else {
            $this.addClass('scrollbar-controls');

            // Resize/re-position the controls.
            $this.siblings('.scroll-prev').css({
              top: this.offsetTop,
              left: this.offsetLeft,
              height: this.offsetHeight
            });
            $this.siblings('.scroll-next').css({
              top: this.offsetTop,
              right: $this.parent().outerWidth() - this.offsetWidth - this.offsetLeft + parseFloat($this.parent().css('padding-right')) - parseFloat($this.parent().css('padding-left')),
              height: this.offsetHeight
            });
          }

        }

        $this.trigger('scroll');
      });
    },
    onScroll: function() {
      var $this = $(this);
      var $prev = $this.siblings('.scroll-prev').hide();
      var $next = $this.siblings('.scroll-next').hide();

      if (this.scrollWidth > this.offsetWidth) {
        if (this.scrollLeft !== 0) {
          $prev.show();
        }

        var scrollLeftMax = this.scrollLeftMax || (this.scrollWidth - this.offsetWidth);
        if (this.scrollLeft !== scrollLeftMax) {
          $next.show();
        }
      }
    }
  };

})(jQuery, Drupal, Drupal.debounce);
