#include <array>
#include <cstddef>
#include <fstream>
#include <string>

#include <argp.h>
#include <reelay/monitors.hpp>
#include <sys/types.h>

// argp option keys
enum RYBINX_OPTS : uint8_t { OPT_DENSE = 'v', OPT_DISCRETE = 'x' };

const char* argp_program_version = "rybinx 0.1.0";
const char* argp_program_bug_address = "Dogan Ulus <github.com/doganulus>";
static const char* doc = "Reelay on Timescales Raw Binary Format";
static const char* args_doc = "SPEC FILE";

struct arguments {
  char* spec;
  char* file;
  bool dense = false;
  bool discrete = false;
};

static std::array<struct argp_option, 12> options = {
  {{"dense", OPT_DENSE, nullptr, 0, "Use dense time model (default)", 0},
   {"discrete", OPT_DISCRETE, nullptr, 0, "Use discrete time model", 0},
   {nullptr}}};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
  auto* arguments = (struct arguments*)state->input;
  switch(key) {
    case OPT_DENSE:
      arguments->dense = true;
      break;
    case OPT_DISCRETE:
      arguments->discrete = true;
      break;
    case ARGP_KEY_ARG:
      if(state->arg_num == 0) {
        arguments->spec = arg;
      }
      else {
        arguments->file = arg;
      }
      break;
    case ARGP_KEY_END:
      if(state->arg_num < 2) {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
static struct argp argp = {options.data(), parse_opt, args_doc, doc};

#pragma pack(push, 1)
struct tx_binary_input_t {
  int32_t time;
  bool p;
  bool q;
  bool r;
  bool s;
};
#pragma pack(pop)
static_assert(std::is_trivially_copyable_v<tx_binary_input_t>);

template<>
struct reelay::timefield<int32_t, tx_binary_input_t> {
  using input_t = tx_binary_input_t;
  inline static int32_t get_time(const input_t& msg)
  {
    return msg.time;
  }
};

template<>
struct reelay::datafield<tx_binary_input_t> {
  using input_t = tx_binary_input_t;

  inline static input_t at(
    const input_t& /*container*/, const std::string& /*key*/)
  {
    return tx_binary_input_t{};
  }

  inline static input_t at(const input_t& /*container*/, std::size_t /*index*/)
  {
    return tx_binary_input_t{};
  }

  inline static bool contains(
    const input_t& /*container*/, const std::string& /*key*/)
  {
    return true;
  }

  inline static bool as_bool(const input_t& msg, const std::string& key)
  {
    bool result = false;
    if(key == "p") {
      result = msg.p;
    }
    else if(key == "q") {
      result = msg.q;
    }
    else if(key == "r") {
      result = msg.r;
    }
    else if(key == "s") {
      result = msg.s;
    }
    else {
      throw std::out_of_range("Key not found");
    }
    return result;
  }

  inline static int64_t as_integer(
    const input_t& /*container*/, const std::string& /*key*/)
  {
    return 0;
  }

  inline static double as_floating(
    const input_t& /*container*/, const std::string& /*key*/)
  {
    return 0.0;
  }

  inline static std::string as_string(
    const input_t& /*container*/, const std::string& /*key*/)
  {
    return std::string{};
  }

  inline static bool contains(
    const input_t& /*container*/, std::size_t /*index*/)
  {
    return true;
  }

  inline static bool as_bool(
    const input_t& /*container*/, std::size_t /*index*/)
  {
    return false;
  }

  inline static int as_integer(
    const input_t& /*container*/, std::size_t /*index*/)
  {
    return 0;
  }

  inline static double as_floating(
    const input_t& /*container*/, std::size_t /*index*/)
  {
    return 0.0;
  }

  inline static std::string as_string(
    const input_t& /*container*/, std::size_t /*index*/)
  {
    return std::string{};
  }
};

static constexpr size_t buffer_size =
  static_cast<const size_t>(64 * 1024);  // 64 KiB

int main(int argc, char** argv)
{
  struct arguments arguments;
  argp_parse(&argp, argc, argv, 0, nullptr, &arguments);

  using input_t = tx_binary_input_t;
  using output_t = reelay::json;

  // Choices
  bool use_discrete = arguments.discrete;
  bool use_dense = arguments.dense;

  // Apply defaults
  if(!use_discrete && !use_dense) {
    use_discrete = true;
  }

  auto monitor = reelay::monitor<input_t, output_t>();
  auto discrete_opts =
    reelay::discrete_timed<int32_t>::monitor<input_t, output_t>::options()
      .with_condensing(true);
  auto dense_opts =
    reelay::dense_timed<int32_t>::monitor<input_t, output_t>::options();

  if(use_dense) {
    monitor = reelay::make_monitor(arguments.spec, dense_opts);
  }
  else {
    monitor = reelay::make_monitor(arguments.spec, discrete_opts);
  }

  std::string filename = arguments.file;
  std::ifstream input(filename, std::ios::binary);
  if(!input) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return 1;
  }
  std::ofstream output(filename + ".ryl");
  if(!output) {
    std::cerr << "Error creating output" << std::endl;
    return 1;
  }
  std::cout << "Processing " << filename << std::endl;
  std::cout << "---" << std::endl;

  uint32_t total_lines = 0;
  input.read(reinterpret_cast<char*>(&total_lines), sizeof(total_lines));
  if(input.gcount() != sizeof(total_lines)) {
    std::cerr << "File too small or corrupted." << std::endl;
    return 1;
  }

  using record_t = tx_binary_input_t;
  static_assert(
    std::is_trivially_copyable_v<record_t>,
    "record_t must be trivially copyable for binary I/O!");

  const size_t record_size = sizeof(record_t);

  std::vector<char> buffer(buffer_size);
  std::vector<record_t> records;
  records.reserve(buffer_size / record_size);

  uint64_t count = 0;
  uint64_t errn = 0;

  while(true) {
    input.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    std::streamsize bytes_read = input.gcount();
    if(bytes_read <= 0) {
      break;
    }

    size_t num_records = bytes_read / record_size;

    const char* ptr = buffer.data();
    for(size_t i = 0; i < num_records; i++) {
      record_t rec;
      std::memcpy(&rec, ptr, record_size);
      ptr += record_size;

      reelay::json result = monitor.update(rec);
      if(not result.empty()) {
        errn++;
        std::cout << result << std::endl;
      }
      count++;
    }
  }
  std::cout << "---" << std::endl;

  return 0;
}
