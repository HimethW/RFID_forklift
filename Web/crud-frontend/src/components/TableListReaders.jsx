import { useState } from "react";

export default function TableListReaders() {
  const [error, setError] = useState(null);
  const [searchTerm, setSearchTerm] = useState("");
  const [tableData, setTableData] = useState([
    {
      id: 1,
      reader_id: "RD001",
      reader_name: "Forklift Alpha",
      reader_type: "Forklift",
      status: "Working",
      last_picked_up: "2025-05-21T10:15:00Z",
    },
    {
      id: 2,
      reader_id: "RD002",
      reader_name: "Handheld Bravo",
      reader_type: "Handheld",
      status: "Idle",
      last_picked_up: "2025-05-21T12:45:00Z",
    },
    {
      id: 3,
      reader_id: "RD003",
      reader_name: "Forklift Charlie",
      reader_type: "Forklift",
      status: "Maintenance",
      last_picked_up: "2025-05-20T09:30:00Z",
    },
    {
      id: 4,
      reader_id: "RD004",
      reader_name: "Handheld Delta",
      reader_type: "Handheld",
      status: "Working",
      last_picked_up: "2025-05-19T14:20:00Z",
    },
    {
      id: 5,
      reader_id: "RD005",
      reader_name: "Forklift Echo",
      reader_type: "Forklift",
      status: "Idle",
      last_picked_up: "2025-05-18T16:10:00Z",
    },
    {
      id: 6,
      reader_id: "RD006",
      reader_name: "Handheld Foxtrot",
      reader_type: "Handheld",
      status: "Working",
      last_picked_up: "2025-05-17T11:30:00Z",
    },
  ]);

  const filteredData = tableData.filter(
    (client) =>
      client.reader_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      client.reader_name.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const handleDelete = (id) => {
    const confirmDelete = window.confirm(
      "Are you sure you want to delete this reader?"
    );
    if (confirmDelete) {
      setTableData((prevData) => prevData.filter((client) => client.id !== id));
    }
  };

  const handleOpen = (mode, client) => {
    alert(`${mode} clicked for ${client.reader_id}`);
  };

  const toggleStatus = (id) => {
    setTableData((prevData) =>
      prevData.map((reader) =>
        reader.id === id
          ? {
              ...reader,
              status: reader.status === "Working" ? "Idle" : "Working",
            }
          : reader
      )
    );
  };

  return (
    <>
      <div className="p-6">
        {error && <div className="alert alert-error">{error}</div>}

        <div className="overflow-x-auto mt-6">
          <table className="table ml-6">
            <thead>
              <tr>
                <th>Reader ID</th>
                <th>Reader Name</th>
                <th>Reader Type</th>
                <th>Last Picked Up</th>
                <th>Status</th>
                <th>Update</th>
                <th>Delete</th>
              </tr>
            </thead>
            <tbody className="hover">
              {filteredData.map((client) => (
                <tr key={client.id}>
                  <td>{client.reader_id}</td>
                  <td>{client.reader_name}</td>
                  <td>{client.reader_type}</td>
                  <td>{client.last_picked_up}</td>
                  <td>
                    <button
                      className={`btn rounded-full w-24 ${
                        client.status === "Working"
                          ? "btn-success"
                          : "btn-warning"
                      }`}
                      onClick={() => toggleStatus(client.id)}
                    >
                      {client.status}
                    </button>
                  </td>
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
      </div>
    </>
  );
}
