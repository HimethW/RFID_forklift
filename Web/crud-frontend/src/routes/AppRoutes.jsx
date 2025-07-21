import { Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import Readers from "./pages/readers";
import ReaderPage from "../pages/ReaderPage";
import Users from "../pages/Users";

const AppRoutes = () => {
  return (
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/readers" element={<Readers />} />
      <Route path="/users" element={<Users />} />
      <Route path="/readers" element={<Readers />} />
      <Route path="/readerPage" element={<ReaderPage />} />
    </Routes>
  );
};

export default AppRoutes;
