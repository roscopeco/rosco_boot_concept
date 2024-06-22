#!/usr/bin/env ruby
#
# Calc rects for Xosera blitter fill with non-aligned rectangles
#
# For every problem, there exists a solution which is simple, obvious and wrong...
#

def blit_rects(x, w)
  x_end = x + w - 1
  
  xr2 = x >> 2
  xr2l2 = xr2 << 2
  xendr2l2 = x_end >> 2 << 2
  
  x_bound = x - xr2l2
  x_end_bound = x_end - xendr2l2

  if x_bound == 0
    # start is on a boundary
    main_rect_x = xr2l2
  else
    main_rect_x = xr2 + 4
  end

  if x_end_bound == 3
    # end is on a boundary
    main_rect_x_end = xendr2l2 + 3
  else
    main_rect_x_end = xendr2l2 - 1
  end

  main_rect_w = main_rect_x_end - main_rect_x + 1

  if main_rect_w < 4
    main_rect_x = -1
    main_rect_x_end = -1
    main_rect_w = 0
    main_rect_const = 0x0000
  else
    main_rect_const = 0xFFFF
  end

  # left rect

  if x_bound == 0
    #start is on a boundary - no left rect
    left_rect_x = -1
    left_rect_w = 0
    left_rect_const = 0x0000
  else
    left_rect_x = xr2
    left_rect_w = 4

    if x_bound == 1
      left_rect_const = 0x0FFF
    elsif x_bound == 2
      left_rect_const = 0x00FF
    elsif x_bound == 3
      left_rect_const = 0x000F
    else
      puts "WTAF?"
    end
  end

  # right rect
  if x_end_bound == 3
    # end is on a boundary - no right rect
    right_rect_x = -1
    right_rect_w = 0
    right_rect_const = 0x0000
  else
    right_rect_x = xendr2l2
    right_rect_w = 4

    if x_end_bound == 0
      right_rect_const = 0xF000
    elsif x_bound == 1
      right_rect_const = 0xFF00
    elsif x_bound == 2
      right_rect_const = 0xFFF0
    else
      puts "WTAF?"
    end
  end


  puts "full  rect is { x: #{x}, xe: #{x_end}, w: #{w} }"
  puts "left  rect is { x: #{left_rect_x}, w: #{left_rect_w} } = #{"0x%04x" % left_rect_const}"
  puts "main  rect is { x: #{main_rect_x}, w: #{main_rect_w} } = #{"0x%04x" % main_rect_const}"
  puts "right rect is { x: #{right_rect_x}, w: #{right_rect_w} } = #{"0x%04x" % right_rect_const}"

  if left_rect_w == 0
    puts("<no left rect>")
  else
    left_blit_start = left_rect_x >> 2
    left_blit_words = left_rect_w >> 2

    puts("left : BLIT_DST_D = #{"0x%04x" % left_blit_start}; BLIT_WORDS = #{"0x%04x" % left_blit_words}; BLIT_SRC_S = #{"0x%04x" % left_rect_const}")
  end

  if main_rect_w == 0
    puts("<no main rect>")
  else
    main_blit_start = main_rect_x >> 2
    main_blit_words = main_rect_w >> 2

    puts("main : BLIT_DST_D = #{"0x%04x" % main_blit_start}; BLIT_WORDS = #{"0x%04x" % main_blit_words}; BLIT_SRC_S = #{"0x%04x" % main_rect_const}")
  end

  if right_rect_w == 0
    puts("<no right rect>")
  else
    right_blit_start = right_rect_x >> 2
    right_blit_words = right_rect_w >> 2

    puts("right: BLIT_DST_D = #{"0x%04x" % right_blit_start}; BLIT_WORDS = #{"0x%04x" % right_blit_words}; BLIT_SRC_S = #{"0x%04x" % right_rect_const}")
  end
end

blit_rects(1, 9)
blit_rects(1, 13)
blit_rects(0, 4)
blit_rects(3, 2)
blit_rects(3, 1)
blit_rects(4, 4)


