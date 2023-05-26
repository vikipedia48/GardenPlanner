use rand::Rng;

/// Generates a unique ID for tracing purposes.
pub fn generate_id() -> String {
    let mut rng = rand::thread_rng();
    let chars: Vec<char> = "abcdefghijklmnopqrstuvwxyz0123456789".chars().collect();
    let timestamp = chrono::Utc::now().timestamp();
    let s = (0..4)
        .map(|_| chars[rng.gen_range(0..chars.len())])
        .collect::<String>();

    format!("{}-{}", timestamp, s)
}
