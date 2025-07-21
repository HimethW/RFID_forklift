import { useState } from "react";
import { useNavigate } from "react-router-dom";
import axios from "axios";
import { jwtDecode } from "jwt-decode";
import "../Login.css";

export default function Login() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const navigate = useNavigate();

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      // Send to your Node/Express backend
      const res = await axios.post("http://localhost:3000/api/login", {
        username,
        password,
      });
      const { token } = res.data;
      console.log("front :", token);

      // Save token
      localStorage.setItem("token", token);

      // Decode role
      const decoded = jwtDecode(token);
      const role = decoded.role;

      // Redirect based on role
      if (role === "Admin") {
        navigate("/shipments"); // Or your admin dashboard route
      } else if (role === "Client") {
        navigate("/readerPage"); // Or your client dashboard route
      } else {
        setError("Unknown user role");
      }
    } catch (err) {
      console.error(err);
      setError("Invalid credentials");
    }
  };

  return (
    <div className="login-container">
      <form className="login-form" onSubmit={handleSubmit}>
        <h2 className="login-title">Login</h2>
        {error && <p className="login-error">{error}</p>}
        <input
          type="text"
          placeholder="Username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          required
        />
        <input
          type="password"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          required
        />
        <button type="submit">Login</button>
      </form>
    </div>
  );
}
