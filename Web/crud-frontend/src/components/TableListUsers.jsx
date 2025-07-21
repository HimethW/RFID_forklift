import axios from "axios";
import { useState } from "react";

export default function TableListUsers({
  handleOpen,
  tableData,
  setTableData,
  searchTerm,
}) {
  const [error, setError] = useState(null);

  // Filter the tableData based on the searchTerm
  const filteredData = tableData.filter(
    (client) =>
      client.user_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.role.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const handleDelete = async (id) => {
    const confirmDelete = window.confirm(
      "Are you sure you want to delete this user?"
    );
    if (confirmDelete) {
      try {
        await axios.delete(`http://localhost3000/api/users/${id}`); // API call to delete client
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
              <th>User ID</th>
              <th>Name</th>
              <th>Role</th>
              <th>Status</th>
            </tr>
          </thead>
          <tbody className="hover">
            {/* row 1 */}

            {filteredData.map((client) => (
              <tr key={client.id}>
                <th>{client.id}</th>
                <td>{client.user_id}</td>
                <td>{client.name}</td>
                <td>{client.role}</td>
               
                <td>
                  <button
                    className={`btn rounded-full w-20 ${
                      client.isactive
                        ? `btn-primary`
                        : `btn-outline btn-primary`
                    }`}
                  >
                    {client.isactive ? "Active" : "Inactive"}
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
