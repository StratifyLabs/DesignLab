#include <sys.hpp>

#include "Sl.hpp"

Sl::Sl() : m_path(Process::Environment::which("sl") ){}

Sl &Sl::launch(const sys::Process::Arguments &arguments) {
  //don't start sl if it is already running

  API_ASSERT(m_state == State::completed);

  m_process = Process(
    Process::Arguments(arguments).push("--json"),
    Process::Environment());

  auto fd = m_process.pipe().read_file().fileno();
  ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

  m_state = State::running;
  return *this;
}
