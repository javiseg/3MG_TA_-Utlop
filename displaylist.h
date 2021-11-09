#pragma once
#include <memory>
#include <list>

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
			auto cmd = std::make_unique<ClearComand>();
			cmd->r = r;
			cmd->g = g;
			cmd->b = b;
			cmd->a = a;
			//_commands.push_back(std::move(cmd));

			return *this;
		}

		DisplayList& updateBuffer(int buffer, const void* mem, int size) {
			auto cmd = std::make_unique<UpdateBufferCommand>(buffer, mem, size);

			_commands.push_back(std::move(cmd));

			return *this;
		}

		DisplayList& render() {
			auto cmd = std::make_unique<RenderComand>();

			_commands.push_back(std::move(cmd));

			return *this;
		}


		DisplayList& swap() {
			auto cmd = std::make_unique<SwapCommand>();

			_commands.push_back(std::move(cmd));

			return *this;
		}


		void executeOnGPU() {

			for (auto&& i : _commands) {
				i.executeOnGPU();
			}

		}

		struct Command {

			virtual ~Command();
			virtual void executeOnGPU() = 0;
			//virtual void push_back();
		};

		struct UpdateBufferCommand : public Command {

			virtual void executeOnGPU() {}
			UpdateBufferCommand(int buffer, const void* mem, const size_t size) : buffer(buffer) {
				data = std::make_unique<uint8_t[]>(size);
				memcpy(data.get(), mem, size);
			}
			std::unique_ptr<uint8_t[]> data;
			int buffer;
		};

		struct RenderComand {


		};
		struct ClearComand : public Command {


			virtual void executeOnGPU() {  }
			float r, g, b, a;
		};

		struct RenderComand : public Command {


			virtual void executeOnGPU() { }
			float r, g, b, a;
		};

		struct SwapCommand : public Command {


			virtual void executeOnGPU() { }
			float r, g, b, a;
		};

	private:
		std::list<Command> _commands;


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
