const projectsDisplay = document.getElementById("projects-display");

displayProjects();

async function displayProjects() {
    let projectObjs = await getProjects('/data/projects.json');
    if (! projectObjs || projectObjs.length === 0) {
        return;
    }

    const projectsDisplayColumn = (projectObj) => {
        return `
        <div class="col-md-4">
            <div class="py-3">
                ${projectCard(projectObj)}
            </div>
        </div>
        `;
    }
    
    let projectsDisplayUI = "";
    projectObjs.forEach(projectObj => {
        projectsDisplayUI += projectsDisplayColumn(projectObj);
    });

    projectsDisplay.innerHTML = projectsDisplayUI;
}


/**
 * 
 * @param {string} projectJsonFilename The filename of the JSON file with the projects data
 * @returns {Promise<{
 *  title: string,
 *  description: string,
 *  img: string,
 *  "github-link": string,
 *  "demo-link": string,
 *  "tags": string[]
 * }[]|false>}
 * 
 */
async function getProjects(projectJsonFilename) {

    return fetch(projectJsonFilename)
        .then(response => {
            // Check if fetch was successful
            if (! response.ok) {
                console.error('Something went wrong: ' + response.statusText);
                return false;
            }

            return response.json();
        })
        .then(data => {
            return data;
        })
        .catch(error => {
            console.error('Could not fetch the projects', error);
            return false;
        });
}


/**
 * 
 * @param { {
 *  title: string,
 *  description: string,
 *  img: string,
 *  "github-link": string,
 *  "demo-link": string,
 *  tags: string[]
 * } } projectObj 
 * @returns {string} The HTML UI Components
 */
function projectCard(projectObj) {
    return `
    <!-- Project Card -->
    <div class="card project-card">
        <!-- Card Project image -->
        <div class="project-img">
            <img src="/imgs/${projectObj.img}" alt="" class="card-img-top img-fluid" height="100" />
        </div>

        <!-- Card header -->
        <div class="card-header">
            <h5 class="card-title">${projectObj.title}</h5>
        </div>

        <!-- Card body -->
        <div class="card-body">
            <p class="card-text small">
                ${projectObj.description}
            </p>
        </div>

        <!-- Card footer -->
        <div class="card-footer d-flex align-content-center justify-content-between py-3">
            <a href="${projectObj["github-link"]}" class="btn btn-sm btn-outline-primary">Go to Github</a>
            <a href="${projectObj["demo-link"]}" class="btn btn-sm btn-outline-secondary">View Demo</a>
        </div>
    </div>`;
}