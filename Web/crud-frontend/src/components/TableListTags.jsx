import axios from "axios";
import { useState } from "react";

export default function TableListTags({
  handleOpen,
  tableData,
  setTableData,
  searchTerm,
}) {
  const [error, setError] = useState(null);
  console.log(tableData);

  // Filter the tableData based on the searchTerm
  const filteredData = tableData.filter(
    (client) =>
      client.tag_id?.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.description?.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const handleDelete = async (tag_id) => {
    const confirmDelete = window.confirm(
      "Are you sure you want to delete this tag?"
    );
    if (confirmDelete) {
      try {
        await axios.delete(`http://localhost3000/api/tags/${tag_id}`); // API call to delete client
        setTableData((prevData) =>
          prevData.filter((client) => client.tag_id !== tag_id)
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
              <th>tag ID</th>
              <th>Description</th>
              <th>Created at</th>
            </tr>
          </thead>
          <tbody className="hover">
            {/* row 1 */}

            {filteredData.map((client) => (
              <tr key={client.tag_id}>
                <td>{client.tag_id}</td>
                <td>{client.description}</td>
                <td>{client.created_at}</td>

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
                    onClick={() => handleDelete(client.tag_id)}
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
