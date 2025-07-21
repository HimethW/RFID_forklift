import { StrictMode } from 'react'
import './index.css'

import ReactDOM from "react-dom/client";
import { BrowserRouter,Routes,Route} from 'react-router-dom';
import Readers from './pages/readers.jsx';
import Warehouse from './pages/Warehouse.jsx';
import Tags from './pages/Tags.jsx';
import Users from './pages/Users.jsx';
import Shipments from "./pages/Shipments.jsx"
import Login from './pages/Login.jsx';
import ReaderPage from './pages/ReaderPage.jsx';


const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(
  <StrictMode>
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Login />} />
        <Route path="/readerPage" element={<ReaderPage />} />
        <Route path="/shipments" element={<Shipments />} />
        <Route path="/readers" element={<Readers />} />
        <Route path="/warehouse" element={<Warehouse />} />
        <Route path="/tags" element={<Tags />} />
        <Route path="/users" element={<Users />} />
      </Routes>
    </BrowserRouter>
  </StrictMode>
);
