import { useState, useEffect } from "react"

export default function ModalFormReaders({ isOpen , onClose, mode, OnSubmit,clientData}) {
    const [reader_id, setreader_id] = useState("");
    const [reader_name, setreader_name] = useState("");
    const [reader_type, setreader_type] = useState("");
    const [status, setStatus] = useState('');
    const [last_picked_up, setLast_picled_up] = useState("");

    // Handle the change of status
    const handleStatusChange = (e) => {
        setStatus(e.target.value === 'Active'); // Set status as boolean
    }

    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const clientData = {reader_id, reader_name, reader_type, status , last_picked_up}
            await OnSubmit(clientData)
            onClose();
        } catch (err) {
            console.error("Error adding reader" , err);
        }
        
    }

    useEffect(() => {
        if (mode === 'edit' && clientData) {
            setreader_id(clientData.reader_id);
            setreader_name(clientData.reader_name);
            setreader_type(clientData.reader_type);
            setStatus(clientData.status);
            setLast_picled_up(clientData.last_picked_up); // Assuming isActive is a boolean
        } else {
            // Reset fields when adding a new client
            setreader_id('');
            setreader_name("");
            setreader_type("");
            setStatus("");
            setLast_picled_up("");
        }
    }, [mode, clientData]);


    return (



        
        <>
            {/* You can open the modal using document.getElementById('ID').showModal() method */}
            <dialog id="my_modal_3" className="modal" open={isOpen}>
            <div className="modal-box">
                <h3 className="font-bold text-lg py-4">{mode === 'edit' ? 'Edit Client' : 'Client Details' }</h3>
                <form method="dialog" onSubmit={handleSubmit} >
                {/* if there is a button in form, it will close the modal */}

                <label className="input input-bordered my-4 flex items-center gap-2">
                    Reader ID
                    <input type="text" className="grow" value={reader_id} onChange={(e) => setreader_id(e.target.value)}/>
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Reader Name
                    <input type="text" className="grow" value={reader_name} onChange={(e) => setreader_name(e.target.value)} />
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Reader Type
                    <input type="text" className="grow" value={reader_type} onChange={(e) => setreader_type(e.target.value)}/>
                </label>

                <label className="input input-bordered my-4 flex items-center gap-2">
                    Reader Type
                    <input type="text" className="grow" value={reader_type} onChange={(e) => setreader_type(e.target.value)}/>
                </label>

                <label className="input input-bordered my-4 flex items-center gap-2">
                    Reader Type
                    <input type="text" className="grow" value={reader_type} onChange={(e) => setreader_type(e.target.value)}/>
                </label>




                <div className="flex mb-4 justify-between my-4">
                    <label className="input input-bordered mr-4 flex items-center gap-2">
                        Rate
                        <input type="number" className="grow" value={rate} onChange={(e) => setRate(e.target.value)}/>
                    </label>
                    <select value={status ? 'Active' : 'Inactive'} className="select select-bordered w-full max-w-xs" onChange={handleStatusChange}>
                        <option>Inactive</option>
                        <option>Active</option>
                    </select>


                </div>



                <button type="button" className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2"  onClick={onClose}>✕</button>
                
                <button type="submit" className="btn btn-success"> {mode === 'edit' ? 'Save Changes' : 'Add Reader' }</button>
                </form>
            </div>
            </dialog>
        </>
    )
}

