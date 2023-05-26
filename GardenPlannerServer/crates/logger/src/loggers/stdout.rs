use std::str::FromStr;

use colored::Colorize;
use log::LevelFilter;

pub struct StdoutLogger {
    pub(crate) default_level: LevelFilter,
    pub(crate) module_levels: Vec<(String, LevelFilter)>,
}

impl StdoutLogger {
    #[must_use = "You must call init() to begin logging"]
    pub fn new() -> Self {
        Self {
            default_level: LevelFilter::Error,
            module_levels: Vec::new(),
        }
    }

    #[must_use = "You must call init() to begin logging"]
    pub fn with_env(mut self) -> Self {
        self.default_level = std::env::var("RUST_LOG")
            .ok()
            .as_deref()
            .map(log::LevelFilter::from_str)
            .and_then(Result::ok)
            .unwrap_or(self.default_level.clone());

        self
    }

    #[must_use = "You must call init() to begin logging"]
    pub fn with_level(mut self, level: LevelFilter) -> Self {
        self.default_level = level;
        self
    }
    
    #[must_use = "You must call init() to begin logging"]
    pub fn with_module_level(mut self, target: &str, level: LevelFilter) -> Self {
        self.module_levels.push((target.to_string(), level));
        self
    }

	pub fn init(mut self) {
        #[cfg(windows)]
        set_up_color_terminal();

        self.module_levels.sort_by_key(|(name, _)| name.len().wrapping_neg());
        let max_level = self.module_levels.iter().map(|(_, level)| level).max();
        let max_level = max_level.map(|level| level.max(&self.default_level)).unwrap_or(&self.default_level).clone();

		log::set_max_level(max_level);
		log::set_boxed_logger(Box::new(self)).unwrap();
	}
}

impl log::Log for StdoutLogger {
    fn enabled(&self, metadata: &log::Metadata) -> bool {
        let record_level = metadata.level().to_level_filter();
        let applicable_level = self.module_levels
            .iter()
            .find(|(name, _)| metadata.target().starts_with(name))
            .map(|(_, level)| level)
            .unwrap_or(&self.default_level)
            .clone();

        record_level <= applicable_level
    }

    fn log(&self, record: &log::Record) {
        if !self.enabled(record.metadata()) { return }

        let now = format!("[{}]", chrono::Local::now().format("%d-%m-%Y %H:%M:%S%.3f")).bright_black();

		let level = match record.level() {
			log::Level::Error => "ERROR".red(),
			log::Level::Warn => " WARN".yellow(),
			log::Level::Info => " INFO".cyan(),
			log::Level::Debug => "DEBUG".purple(),
			log::Level::Trace => "TRACE".normal(),
		};

        println!("{} {} | {}", now, level, record.args());
    }

    fn flush(&self) {}
}

#[cfg(windows)]
fn set_up_color_terminal() {
    use atty::Stream;

    if atty::is(Stream::Stdout) {
        unsafe {
            use windows_sys::Win32::Foundation::INVALID_HANDLE_VALUE;
            use windows_sys::Win32::System::Console::{
                GetConsoleMode, GetStdHandle, SetConsoleMode, CONSOLE_MODE, ENABLE_VIRTUAL_TERMINAL_PROCESSING,
                STD_OUTPUT_HANDLE,
            };

            let stdout = GetStdHandle(STD_OUTPUT_HANDLE);

            if stdout == INVALID_HANDLE_VALUE {
                return;
            }

            let mut mode: CONSOLE_MODE = 0;

            if GetConsoleMode(stdout, &mut mode) == 0 {
                return;
            }

            SetConsoleMode(stdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
}