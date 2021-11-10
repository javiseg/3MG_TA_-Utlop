#pragma once
#include <memory>
#include <list>
#include <vector>

struct vec3 {

	float x, y, z;
	vec3(float xx, float yy, float zz) {

		x = xx;
		y = yy;
		z = zz;
	}

};

namespace Utlop {

	class DisplayList {

		public:

		DisplayList& clear(float r, float g, float b, float a = 1.0f) {
			auto cmd = std::make_unique<ClearCommand>();
			cmd->r = r;
			cmd->g = g;
			cmd->b = b;
			cmd->a = a;
			//_commands.push_back(std::move(cmd));

			return *this;
		}

		DisplayList& updateBuffer(int buffer, const void* mem, int size) {
			auto cmd = std::make_unique<UpdateBufferCommand>(buffer, mem, size);

			//_commands.push_back(std::move(cmd));

			return *this;
		}

		DisplayList& render() {
			auto cmd = std::make_unique<RenderCommand>();

			//_commands.push_back(std::move(cmd));

			return *this;
		}


		DisplayList& swap() {
			auto cmd = std::make_unique<SwapCommand>();

			//_commands.push_back(std::move(cmd));

			return *this;
		}


		void executeOnGPU() {

			for (auto&& i : _commands) {
				//i.executeOnGPU();
			}

		}
		struct GPUState {
			int number;
		};
		struct Command {
			
			Command() = default;
			Command(Command&&) = default;
			Command& operator=(Command&&) = delete;
			virtual ~Command();
			virtual void executeOnGPU(GPUState*) = 0;
			
		};

		struct UpdateBufferCommand : public Command {

			void executeOnGPU(GPUState*) {}

			UpdateBufferCommand(int buffer, const void* mem, const size_t size) : buffer(buffer) {
				data = std::make_unique<uint8_t[]>(size);
				memcpy(data.get(), mem, size);
			}
			std::unique_ptr<uint8_t[]> data;
			int buffer;
		};

		struct ClearCommand : public Command {

			void executeOnGPU(GPUState*) {  }
			float r, g, b, a;
		};

		struct RenderCommand : public Command {

			void executeOnGPU(GPUState*) { }
			float r, g, b, a;
		};

		struct SwapCommand : public Command {

			void executeOnGPU(GPUState*) { }
			float r, g, b, a;
		};
		struct DrawGeometry : public Command {

			void executeOnGPU(GPUState*);
		};
	private:
		std::list<std::unique_ptr<Command>> _commands;


	};


	void submit(DisplayList&& list) {


		/*if (list.endWithSwap()) {
		}*/

		//list.executeOnGPU();
	}

	void submit(DisplayList&& list, int texture) {

		list.executeOnGPU();
	}
}
