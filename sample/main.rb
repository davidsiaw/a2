# frozen_string_literal: true

require './system.rb'

puts 'hello'
begin
  sys = System.new(1024, 768)
  sys.window_title = 'The Living Fossil'

  font = sys.load_font('sample.ttf')
  text = font.draw_string('がんばルビィ！', 20, Color.Red)

  postext = sys.centered_image(text)

  music = sys.load_music('sample.flac')
  music.play

  img = sys.load_image('sample.webp')

  posimg = sys.centered_image(img)
  posimg.x = 0
  posimg.y = 0
  p posimg

  sprimg = sys.load_image('x36.png')

  (0..3).each do |i|
    sprite = AnimatedSprite.new(sprimg, 0, i * 64 * 4, 48, 64)
    sprite.yframe = 2
    # sprite xframe = 0

    posspr = sys.centered_image(sprite)
    sys.set_image 3 - i, posspr
  end

  p postext.class
  postext.y += 40
  sys.set_image -4, postext
  sys.set_image -100, posimg

  start = postext.y
  count = 0

  sys.set_image(-100, posimg)
  sys.begin_loop do |x|
    return false if x.type == :quit

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
