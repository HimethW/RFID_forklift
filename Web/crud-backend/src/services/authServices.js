import { query } from "../db.js";

export async function findUserByUsername(username) {
  const result = await query("SELECT * FROM users WHERE name = $1", [
    username,
  ]);
  return result.rows[0];
}
  