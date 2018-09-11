require "./system"


begin
	sys = System.new(1024,768)

	font = sys.load_font("sample.ttf")
	text = font.draw_string("ハロー何を言ってんの what", 80, Color.Red)

	postext = sys.centered_image(text);

	music = sys.load_music("sample.mp3")
	music.play

	img = sys.load_image("sample.png")
	posimg = sys.centered_image(img)

	sys.set_image 0, posimg
	sys.set_image 1, postext

	sys.begin_loop do |x|
		return false if x.type == :quit
		p x.type
		p x.gettype
		return true;
	end

rescue => e
	puts e
end
