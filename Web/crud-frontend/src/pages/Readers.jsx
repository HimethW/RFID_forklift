//import { BrowserRouter as Router,Routes,Route,Link} from 'react-router-dom';
//import Readers from ''./pages/Readers'
import { useState, useEffect } from "react";

import ModalFormReaders from "../components/ModalFormReaders";
import NavBarReaders from "../components/NavBarReaders";
import TableListReaders from "../components/TableListReaders";
import axios from "axios";

function Readers() {
  const [isOpen, setIsOpen] = useState(false);
  const [modalMode, setModalMode] = useState("add");
  const [searchTerm, setSearchTerm] = useState("");
  const [clientData, setClientData] = useState(null);
  const [tableData, setTableData] = useState([]);

  const fetchClients = async () => {
    try {
      const response = await axios.get("http://localhost:3000/api/readers");
      setTableData(response.data); // Set the fetched data
    } catch (err) {
      setError(err.message);
    }
  };

  useEffect(() => {
    fetchClients();
  }, []);

  const handleOpen = (mode, client) => {
    setClientData(client);
    setModalMode(mode);
    setIsOpen(true);
  };
  const handleSubmit = async (newClientData) => {
    if (modalMode === "add") {
      try {
        const response = await axios.post(
          "http://localhost:3000/api/readers",
          newClientData
        ); // Replace with your actual API URL
        console.log("Reader added:", response.data); // Log the response
        setTableData((prevData) => [...prevData, response.data]);
        // Optionally, update your state here to reflect the newly added client
      } catch (error) {
        console.error("Error adding reader:", error); // Log any errors
      }
      console.log("modal mode Add");
    } else {
      console.log("Updating reader with ID:", clientData.id); // Log the ID being updated
      try {
        const response = await axios.put(
          `http://localhost3000/api/readers/${clientData.id}`,
          newClientData
        );
        console.log("Reader updated:", response.data);
        setTableData((prevData) =>
          prevData.map((client) =>
            client.id === clientData.id ? response.data : client
          )
        );
      } catch (error) {
        console.error("Error updating reader:", error);
      }
    }
  };

  return (
    <>
      {/* <Readers />
      <div className="App">
        <header className="App-header">
          <button className="users-button" onClick={() => navigate("/readers")}>Readers</button>
        </header>
      </div> */}
      {/* <Router>
      <Routes>
                <Route path="/readers" element={<Readers />} />
      </Routes>
      </Router>
 */}

      <NavBarReaders onOpen={() => handleOpen("add")} onSearch={setSearchTerm} />
      <TableListReaders
        
      />

    </>
  );
}

export default Readers;
