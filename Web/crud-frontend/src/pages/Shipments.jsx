import { useState } from "react";
import NavBar from "../components/NavShipRead";

export default function Shipments() {
  const [searchTerm, setSearchTerm] = useState("");
  const [tableData, setTableData] = useState([
    {
      id: 1,
      shipment_id: 101,
      tag_id: "TG1001",
      shelf_id: "SH01",
      reader_id: "RD001",
      user_id: "USR01",
      status: "Arrived",
      time: "2025-05-21T10:00:00Z",
    },
    {
      id: 2,
      shipment_id: 102,
      tag_id: "TG1002",
      shelf_id: "SH02",
      reader_id: "RD002",
      user_id: "USR02",
      status: "Dispatched",
      time: "2025-05-22T11:30:00Z",
    },
    {
      id: 3,
      shipment_id: 103,
      tag_id: "TG1003",
      shelf_id: "SH01",
      reader_id: "RD003",
      user_id: "USR03",
      status: "Shelved",
      time: "2025-05-20T15:45:00Z",
    },
    // Added more entries below
    {
      id: 4,
      shipment_id: 104,
      tag_id: "TG1004",
      shelf_id: "SH03",
      reader_id: "RD004",
      user_id: "USR04",
      status: "Arrived",
      time: "2025-05-23T09:00:00Z",
    },
    {
      id: 5,
      shipment_id: 105,
      tag_id: "TG1005",
      shelf_id: "SH02",
      reader_id: "RD002",
      user_id: "USR05",
      status: "Dispatched",
      time: "2025-05-24T12:15:00Z",
    },
    {
      id: 6,
      shipment_id: 106,
      tag_id: "TG1006",
      shelf_id: "SH04",
      reader_id: "RD005",
      user_id: "USR06",
      status: "Shelved",
      time: "2025-05-25T14:30:00Z",
    },
    {
      id: 7,
      shipment_id: 107,
      tag_id: "TG1007",
      shelf_id: "SH05",
      reader_id: "RD006",
      user_id: "USR07",
      status: "Arrived",
      time: "2025-05-26T10:45:00Z",
    },
    {
      id: 8,
      shipment_id: 108,
      tag_id: "TG1008",
      shelf_id: "SH03",
      reader_id: "RD004",
      user_id: "USR08",
      status: "Dispatched",
      time: "2025-05-27T13:00:00Z",
    },
    {
      id: 9,
      shipment_id: 109,
      tag_id: "TG1009",
      shelf_id: "SH01",
      reader_id: "RD001",
      user_id: "USR09",
      status: "Shelved",
      time: "2025-05-28T15:15:00Z",
    },
    {
      id: 10,
      shipment_id: 110,
      tag_id: "TG1010",
      shelf_id: "SH06",
      reader_id: "RD007",
      user_id: "USR10",
      status: "Arrived",
      time: "2025-05-29T08:30:00Z",
    },
  ]);

  const filteredData = tableData.filter(
    (shipment) =>
      shipment.shipment_id.toString().includes(searchTerm) ||
      shipment.tag_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      shipment.shelf_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      shipment.reader_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      shipment.user_id.toLowerCase().includes(searchTerm.toLowerCase()) ||
      shipment.status.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const handleDelete = (id) => {
    if (window.confirm("Are you sure you want to delete this shipment?")) {
      setTableData((prevData) => prevData.filter((item) => item.id !== id));
    }
  };

  return (
    <>
      <NavBar />
      <div className="overflow-x-auto">
        <table className="table ml-6 w-full">
          <thead>
            <tr>
              <th>Shipment ID</th>
              <th>Tag ID</th>
              <th>Shelf ID</th>
              <th>Reader ID</th>
              <th>User ID</th>
              <th>Status</th>
              <th>Time</th>
              <th>Actions</th>
            </tr>
          </thead>
          <tbody className="hover">
            {filteredData.map((shipment) => (
              <tr key={shipment.id}>
                <td>{shipment.shipment_id}</td>
                <td>{shipment.tag_id}</td>
                <td>{shipment.shelf_id}</td>
                <td>{shipment.reader_id}</td>
                <td>{shipment.user_id}</td>
                <td>
                  <span
                    className={`badge ${
                      shipment.status === "Arrived"
                        ? "badge-success"
                        : shipment.status === "Dispatched"
                        ? "badge-warning"
                        : "badge-info"
                    }`}
                  >
                    {shipment.status}
                  </span>
                </td>
                <td>{new Date(shipment.time).toLocaleString()}</td>
                <td>
                  <button
                    onClick={() =>
                      alert(`Edit shipment ${shipment.shipment_id}`)
                    }
                    className="btn btn-secondary btn-sm mr-2"
                  >
                    Edit
                  </button>
                  <button
                    onClick={() => handleDelete(shipment.id)}
                    className="btn btn-error btn-sm"
                  >
                    Delete
                  </button>
                </td>
              </tr>
            ))}
            {filteredData.length === 0 && (
              <tr>
                <td colSpan="8" className="text-center">
                  No shipments found.
                </td>
              </tr>
            )}
          </tbody>
        </table>
      </div>
    </>
  );
}
