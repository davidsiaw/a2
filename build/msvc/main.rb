require "./hello"

begin
	sys = System.new(1024,768)

	music = sys.load_music("sample.mp3")
	music.play

	img = sys.load_image("sample.webp")
	posimg = sys.centered_image(img)

	sys.set_image 0, posimg

	sys.start_event_loop(Proc.new do |x|
		puts x
		return true;
	end)


rescue => e
	puts e
end

$m = Proc.new do
	return 100;
end
