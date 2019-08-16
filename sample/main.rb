require "./system.rb"


puts 'hello'
begin
	sys = System.new(1024,768)
	sys.window_title = "The Living Fossil"

	font = sys.load_font("sample.ttf")
	text = font.draw_string("がんばルビィ！", 20, Color.Red)

	postext = sys.centered_image(text);

	music = sys.load_music("sample.flac")
	music.play

	img = sys.load_image("sample.png")
	
	#posimg = sys.centered_image(img)

	#sys.set_image 0, posimg

	sprimg = sys.load_image("x36.png")

	for i in 0..3
		sprite = AnimatedSprite.new(sprimg, 0, i*64*4, 48, 64)
		sprite.yframe = 2
		#sprite xframe = 0

		posspr = sys.centered_image(sprite)
		sys.set_image 3-i, posspr		
	end

	p postext.class
	postext.y += 40
	sys.set_image 4, postext

	sys.begin_loop do |x|
		return false if x.type == :quit
		#p x.type
		#p x.gettype
		return true;
	end

rescue => e
	puts e
end


