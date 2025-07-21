import axios from "axios";
import { useState } from "react";

export default function TableListWarehouse({
  handleOpen,
  tableData,
  setTableData,
  searchTerm,
}) {
  const [error, setError] = useState(null);

  // Filter the tableData based on the searchTerm
  const filteredData = tableData.filter(
    (client) =>
      client.shelf_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.shelf_loation.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.warehouse_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.warehouse_name.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const handleDelete = async (id) => {
    const confirmDelete = window.confirm(
      "Are you sure you want to delete this shelf?"
    );
    if (confirmDelete) {
      try {
        await axios.delete(`http://localhost:3000/api/warehouse/${id}`); // API call to delete client
        setTableData((prevData) =>
          prevData.filter((client) => client.id !== id)
        ); // Update state
      } catch (err) {
        setError(err.message); // Handle any errors
      }
    }
  };

  return (
    <>
      {error && <div className="alert alert-error">{error}</div>}

      <div className="overflow-x-auto mt-10">
        <table className="table ml-6">
          {/* head */}
          <thead>
            <tr>
              <th></th>
              <th>Shelf ID</th>
              <th>Shelf Location</th>
              <th>warehouse ID</th>
              <th>Warehouse Name</th>
            </tr>
          </thead>
          <tbody className="hover">
            {/* row 1 */}

            {filteredData.map((client) => (
              <tr key={client.id}>
                <th>{client.id}</th>
                <td>{client.shelf_id}</td>
                <td>{client.shelf_loation}</td>
                <td>{client.warehouse_id}</td>
                <td>{client.warehouse_name}</td>
 
                <td>
                  <button
                    onClick={() => handleOpen("edit", client)}
                    className="btn btn-secondary"
                  >
                    Update
                  </button>
                </td>
                <td>
                  <button
                    className="btn btn-accent"
                    onClick={() => handleDelete(client.id)}
                  >
                    Delete
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </>
  );
}
