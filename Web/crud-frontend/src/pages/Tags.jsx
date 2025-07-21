//import { BrowserRouter as Router,Routes,Route,Link} from 'react-router-dom';
//import Readers from ''./pages/Readers'
import { useState, useEffect } from "react";
import "../App.css";
import ModalFormTags from "../components/ModalFormTags";
import axios from "axios";
import TableListTags from "../components/TableListTags";
import NavBarTags from "../components/NavBarTags";

function Tags() {
  const [isOpen, setIsOpen] = useState(false);
  const [modalMode, setModalMode] = useState("add");
  const [searchTerm, setSearchTerm] = useState("");
  const [clientData, setClientData] = useState(null);
  const [tableData, setTableData] = useState([]);
  const [error,setError] = useState(null);

  const fetchClients = async () => {
    try {
      const response = await axios.get("http://localhost:3000/api/tags");
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
        console.log("POSTING:", newClientData); // Confirm data is correct

        const response = await axios.post(
          "http://localhost:3000/api/tags",
          newClientData
        ); // Replace with your actual API URL
        console.log("RESPONSE:", response.data); // Check backend response

        console.log("Tag added:", response.data); // Log the response
        setTableData((prevData) => [...prevData, response.data]);
        // Optionally, update your state here to reflect the newly added client
      } catch (error) {
        console.error("Error adding tag:", error); // Log any errors
      }
      console.log("modal mode Add");
    } else {
      console.log("Updating tag with ID:", clientData.id); // Log the ID being updated
      try {
        const response = await axios.put(
          `http://localhost:3000/api/tags/${clientData.id}`,
          newClientData
        );
        console.log("Tags updated:", response.data);
        setTableData((prevData) =>
          prevData.map((client) =>
            client.id === clientData.id ? response.data : client
          )
        );
      } catch (error) {
        console.error("Error updating tag:", error);
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

      <NavBarTags
        onOpen={() => handleOpen("add")}
        onSearch={setSearchTerm}
      />
      <TableListTags
        setTableData={setTableData}
        tableData={tableData}
        handleOpen={handleOpen}
        searchTerm={searchTerm}
      />
      <ModalFormTags
        isOpen={isOpen}
        OnSubmit={handleSubmit}
        onClose={() => setIsOpen(false)}
        mode={modalMode}
        clientData={clientData}
      />
    </>
  );
}

export default Tags;
