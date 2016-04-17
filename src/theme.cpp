/*=================================================================================================
   Copyright (c) 2016 Joel de Guzman

   Licensed under a Creative Commons Attribution-ShareAlike 4.0 International.
   http://creativecommons.org/licenses/by-sa/4.0/
=================================================================================================*/
#include <photon/theme.hpp>
#include <nanovg.h>

namespace photon
{
   inline NVGcolor nvgRGBA(color const& c)
   {
      NVGcolor color;
      color.r = c.red;
      color.g = c.green;
      color.b = c.blue;
      color.a = c.alpha;
      return color;
   }

   void theme::draw_panel(rect const& b) const
   {
      double   x = b.left;
      double   y = b.top;
      double   w = b.width();
      double   h = b.height();
      color    c = panel_color;
      double   r = panel_corner_radius;
      rect     sh = panel_shadow_offset;

      // Round Rectangle
      nvgBeginPath(_vg);
      nvgRoundedRect(_vg, x, y, w, h, r);
      nvgFillColor(_vg, nvgRGBA(c));
      nvgFill(_vg);

      // Drop shadow
      nvgBeginPath(_vg);
      nvgRect(_vg, x+sh.left, y+sh.top, w+sh.right, h+sh.bottom);
      nvgRoundedRect(_vg, x, y, w, h, r);
      nvgPathWinding(_vg, NVG_HOLE);

      NVGpaint shadow_paint
         = nvgBoxGradient(
               _vg, x, y+2, w, h, r*2, 10
             , ::nvgRGBA(0, 0, 0, 128), ::nvgRGBA(0, 0, 0, 0)
            );

      nvgFillPaint(_vg, shadow_paint);
      nvgFill(_vg);
   }

   namespace
   {
      void draw_slot(NVGcontext* vg, double x, double y, double w, double h, double r)
      {
         // Slot
         NVGpaint bg
            = nvgBoxGradient(
                  vg, x, y+1, w, h, 2, 2,
                  ::nvgRGBA(0, 0, 0, 32), ::nvgRGBA(0, 0, 0, 128)
               );

         nvgBeginPath(vg);
         nvgRoundedRect(vg, x, y, w, h, r);
         nvgFillPaint(vg, bg);
         nvgFill(vg);
      }

      void draw_knob(
         NVGcontext* vg, double cx, double cy, double r,
         color const& fill_color,
         color const& outline_color
      )
      {
         // Knob Shadow
         NVGpaint bg
            = nvgRadialGradient(
                  vg, cx, cy+1, r-3, r+3,
                  ::nvgRGBA(0, 0, 0, 64), ::nvgRGBA(0, 0, 0, 0)
               );

         nvgBeginPath(vg);
         nvgRect(vg, cx-r-5, cy-r-5, r*2+5+5, r*2+5+5+3);
         nvgCircle(vg, cx, cy, r);
         nvgPathWinding(vg, NVG_HOLE);
         nvgFillPaint(vg, bg);
         nvgFill(vg);

         // Knob
         NVGpaint knob
            = nvgLinearGradient(
                  vg, cx, cy-r, cx, cy+r,
                  ::nvgRGBA(255, 255, 255, 16), ::nvgRGBA(0, 0, 0, 16)
               );

         nvgBeginPath(vg);
         nvgCircle(vg, cx, cy, r-1);
         nvgFillColor(vg, nvgRGBA(fill_color));
         nvgFill(vg);
         nvgFillPaint(vg, knob);
         nvgFill(vg);

         nvgBeginPath(vg);
         nvgCircle(vg, cx, cy, r-0.5);
         nvgStrokeColor(vg, nvgRGBA(outline_color));
         nvgStroke(vg);
      }
   }

   void theme::draw_slider(double pos, rect const& b) const
   {
      double   x = b.left;
      double   y = b.top;
      double   w = b.width();
      double   h = b.height();

      nvgSave(_vg);

      if (w > h)
      {
         // horizontal
         double   cy = y + h * 0.5;
         double   sl = h * slider_slot_size;
         draw_slot(_vg, x, cy-(sl/2), w, sl, sl/3);
      }
      else
      {
         // vertical
         double   cx = x + w * 0.5;
         double   sl = w * slider_slot_size;
         draw_slot(_vg, cx-(sl/2), y, sl, h, sl/3);
      }

      draw_slider_knob(pos, b);
      nvgRestore(_vg);
   }

   void theme::draw_slider_knob(double pos, rect const& b) const
   {
      circle cp = slider_knob_position(pos, b);
      draw_knob(
         _vg, cp.cx, cp.cy, cp.radius,
         slider_knob_fill_color, slider_knob_outline_color
      );
   }

   circle theme::slider_knob_position(double pos, rect const& b) const
   {
      double   x = b.left;
      double   y = b.top;
      double   w = b.width();
      double   h = b.height();

      if (w > h)
      {
         // horizontal
         double   cy = y + h * 0.5;
         double   kr = h * slider_knob_radius;

         // inset by radius;
         w -= kr * 2;
         x += kr;

         return { x+(pos*w), cy, kr };
      }
      else
      {
         // vertical
         double   cx = x + w * 0.5;
         double   kr = w * slider_knob_radius;

         // inset by radius;
         h -= kr * 2;
         y += kr;

         return { cx, y+(pos*h), kr };
      }
   }

   void theme::load_fonts() const
   {
      if (nvgCreateFont(_vg, "icons", "./assets/fonts/entypo.ttf") == -1)
      {
         printf("Could not add font icons.\n");
          // $$$ throw $$;
      }

      if (nvgCreateFont(_vg, "sans", "./assets/fonts/Roboto-Regular.ttf") == -1)
      {
         printf("Could not add font italic.\n");
          // $$$ throw $$;
      }

      if (nvgCreateFont(_vg, "sans-bold", "./assets/fonts/Roboto-Bold.ttf") == -1)
      {
         printf("Could not add font bold.\n");
          // $$$ throw $$;
      }
   }
}
