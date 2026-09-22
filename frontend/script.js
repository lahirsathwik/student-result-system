async function searchResult() {

    const rollNumber = document.getElementById("rollNumber").value.trim();

    const resultDiv = document.getElementById("result");

    if (rollNumber === "") {
        resultDiv.innerHTML = "<p>Please enter a roll number.</p>";
        return;
    }

    resultDiv.innerHTML = "<p>Searching...</p>";

    try {

        const response = await fetch(
            `/api/student?roll=${encodeURIComponent(rollNumber)}`
        );

        const data = await response.json();

        if (!response.ok) {
            resultDiv.innerHTML = `<p>${data.message}</p>`;
            return;
        }

     resultDiv.innerHTML = `
    <div class="result-card">

        <h2>📋 Student Result</h2>

        <p><strong>Student Name:</strong> ${data.name}</p>
        <p><strong>Roll Number:</strong> ${data.roll}</p>

        <hr>

        <h3>📚 Subject Marks</h3>

        <p><strong>C:</strong> ${data.c} / 100</p>
        <p><strong>C++:</strong> ${data.cpp} / 100</p>
        <p><strong>Python:</strong> ${data.python} / 100</p>
        <p><strong>DBMS:</strong> ${data.dbms} / 100</p>
        <p><strong>Artificial Intelligence:</strong> ${data.ai} / 100</p>

        <hr>

        <h3>📊 Final Result</h3>

        <p><strong>Total Marks:</strong> ${data.total} / 500</p>
        <p><strong>Percentage:</strong> ${data.percentage}%</p>
        <p><strong>Grade:</strong> ${data.grade}</p>
        <p><strong>Result:</strong> ${data.result}</p>

    </div>
`;
    } catch (error) {

        resultDiv.innerHTML = `
            <p>
                Cannot connect to the C++ backend.
                Please make sure the backend server is running.
            </p>
        `;

        console.error(error);
    }
}