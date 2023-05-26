use digest::Digest;
use rsa::sha2;

pub fn hash_with_sha256(data: &[u8]) -> Vec<u8> {
	let mut hasher = sha2::Sha256::new();
	hasher.update(data);
	hasher.finalize().to_vec()
}