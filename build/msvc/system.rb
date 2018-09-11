puts "hello"

class Event
	def type
		@eventcodes = {
			0x100 => :quit,

			0x150 => :display,

			0x200 => :window,
			0x201 => :system,

			0x300 => :keydown,
			0x301 => :keyup,
			0x302 => :textediting,
			0x303 => :textinput,
			0x304 => :keymapchanged,

			0x400 => :mousemotion,
			0x401 => :mousebuttondown,
			0x402 => :mousebuttonup,
			0x403 => :mousewheel,

			0x800 => :userevent,
		}

		@eventcodes[gettype]
	end
end

class System
	def begin_loop(&block)
		start_event_loop(Proc.new(&block)) if block
	end
end
