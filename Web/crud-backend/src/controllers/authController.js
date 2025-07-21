import jwt from "jsonwebtoken";
import { findUserByUsername } from "../services/authServices.js";

export async function loginUser(req, res) {
  const { username, password } = req.body;
  console.log("➡️ Received login request:", username, password);

  try {
    // 1️⃣ Get user from DB
    const user = await findUserByUsername(username);
    console.log("✅ DB result:", user);

    if (!user) {
      console.log("❌ User not found");
      return res.status(401).json({ message: "Invalid username or password" });
    }

    // 2️⃣ Check plaintext password
    console.log("✅ Comparing passwords:", password, user.password);
    if (password !== user.password) {
      console.log("❌ Passwords don't match");
      return res.status(401).json({ message: "Invalid username or password" });
    }

    // 3️⃣ Create JWT
    const payload = {
      id: user.user_id,
      username: user.name,
      role: user.role,
    };
    console.log("✅ Payload:", payload);
    console.log("✅ JWT_SECRET:", process.env.JWT_SECRET);

    const token = jwt.sign(payload, process.env.JWT_SECRET, {
      expiresIn: "1h",
    });
    console.log("✅ Generated token" + {token});

    res.json({ token });
  } catch (err) {
    console.error("❌ Login error:", err);
    res.status(500).json({ message: "Server error" });
  }
}
