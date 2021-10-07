#ifndef SL_HPP
#define SL_HPP

#include <fs/DataFile.hpp>
#include <fs/ViewFile.hpp>
#include <json/JsonDocument.hpp>
#include <printer/Printer.hpp>
#include <sys/Process.hpp>


class SlResult : public json::JsonObject {
public:
  SlResult(json::JsonObject value) : json::JsonObject(value){}

  bool is_success() const {
    return get_result() == "success";
  }

  JSON_ACCESS_STRING(SlResult, command);
  JSON_ACCESS_STRING_WITH_KEY(SlResult, serialNumber, serial_number);
  JSON_ACCESS_STRING(SlResult, result);

  json::JsonObject get_output() const {
    return at("output");
  }

};


class Sl : public api::ExecutionContext {
public:

  enum class State {
    completed,
    running,
    exited
  };

  Sl();

  sys::Process::Arguments get_arguments() const {
    return sys::Process::Arguments(path());
  }

  Sl &launch(const sys::Process::Arguments &arguments);

  Sl &launch(const var::StringView argument) {
    return launch(get_arguments().push(argument));
  }

  bool is_running(){
    if( m_state != State::running ){
      return false;
    }
    read_pipe();
    const auto result = m_process.is_running();
    if( !result ){
      m_state = State::exited;
    }
    return result;
  }

  Sl &wait() {
    API_ASSERT(m_state == State::running);
    m_process.wait();
    m_state = State::exited;
    read_pipe();
    return *this;
  }

  json::JsonValue get_result() {
    if( m_state != State::exited ){
      return json::JsonValue();
    }

    if( progress_callback() ){
      progress_callback()->update(0, 0);
    }
    m_state = State::completed;
    const auto result = json::JsonDocument().load(pipe_output.seek(0));
    pipe_output = fs::DataFile();
    return result;
  }

  State state() const {
    return m_state;
  }

private:
  API_RAC(Sl, var::PathString, path);
  API_AF(Sl, int, progress, 0);
  API_AF(Sl, const api::ProgressCallback *, progress_callback, nullptr);
  sys::Process m_process;
  fs::DataFile pipe_output;
  State m_state = State::completed;

  void read_pipe() {
    bool is_bytes_read = true;
    while (is_bytes_read) {
      var::Array<char, 4096> buffer;
      api::ErrorScope error_scope;
      auto bytes_read
        = m_process.pipe().read_file().read(buffer).return_value();
      is_bytes_read = bytes_read > 0;
      if (is_bytes_read) {
        for (int i = 0; i < bytes_read; i++) {
          if (buffer.at(i) == '#') {
            // update progress
            m_progress++;

            if( progress_callback() ){
              progress_callback()->update(m_progress, 50);
            }
          }

        }
        pipe_output.write(var::View(buffer).truncate(bytes_read));
      }
    }
  }
};

#endif // SL_HPP
