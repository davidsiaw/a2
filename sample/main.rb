# frozen_string_literal: true

puts "asdasd"

require './sample/system.rb'

begin
    p System.get_video_width(0)
    p System.get_video_height(0)
  sys = System.new(System.get_video_width(0), System.get_video_height(0))
  sys.window_title = 'The Living Fossil'

  font = sys.load_font('sample/sample.ttf')

  music = sys.load_music('sample/sample.flac')
  music.play

  img = sys.load_image('sample/sample.jpg')

  posimg = sys.centered_image(img)
  posimg.x = 0
  posimg.y = 0
  p posimg

  sprimg = sys.load_image('sample/output.png')

  (0..3).each do |i|
    sprite = AnimatedSprite.new(sprimg, 0, i * 64 * 4, 48, 64)
    sprite.yframe = 1
    # sprite xframe = 0

    posspr = sys.centered_image(sprite)
    sys.set_image 3 - i, posspr
  end
  
  text = font.draw_string('がんばルビィ！ IT WORKS', 20, Color.Red)

  postext = sys.centered_image(text)
  postext.y += 40
  sys.set_image -4, postext
  
  sys.set_image -100, posimg

  start = postext.y
  count = 0

  sys.begin_loop do |x|
    return false if x.type == :quit
    
    text = font.draw_string("がんばルビィ！ IT WORKS! #{count.to_s.rjust(2, '0')}", 20, Color.Red)
    postext = sys.centered_image(text)
    sys.set_image -4, postext
    count -= 1
    postext.y = start + count

    count = 0 if count < -100

    # p x.type
    # p x.gettype
    return true

  end
rescue StandardError => e
  puts e
end
