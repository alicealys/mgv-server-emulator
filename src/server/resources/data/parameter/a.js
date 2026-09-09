const fs = require('fs')

const params = JSON.parse(fs.readFileSync('SsdSbmParameters.json').toString())
for (const struct of Object.keys(params))
{
    for (const field of Object.keys(params[struct][0]))
    {
        
    }
}
