// Add this to GLRenderContext.h

public:
	/** This function needs to be called by the SceneManager once per 
		frame, before all objects are rendered. The function assumes 
		your light sources are stored in a std::list.

		NOTE: You also need to add this declaration to the base class 
		RenderContext.h so that the SceneManager can call it.
	*/
	void setLights(const std::list<Light*> &lightList);

private:
	/** This function needs to be called before rendering each object.
	*/
	void setMaterial(Material *material);




// Add the following to GLRenderContext.cpp

void GLRenderContext::setLights(const std::list<Light*> &lightList)
{	
	GLint lightIndex[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::list<Light*>::const_iterator iter;

	if(lightList.begin()!=lightList.end())
	{
		// Lighting
		glEnable(GL_LIGHTING);

		int i=0;
		for (iter=lightList.begin(); iter!=lightList.end() && i<8; iter++)
		{
			i++;
			Light *l = (*iter);

			glEnable(lightIndex[i]);

			if(l->getType() == Light::DIRECTIONAL)
			{
				float direction[4];
				direction[0] = l->getDirection().x;
				direction[1] = l->getDirection().y;
				direction[2] = l->getDirection().z;
				direction[3] = 0.f;
				glLightfv(lightIndex[i], GL_POSITION, direction);
			}
			if(l->getType() == Light::POINT || l->getType() == Light::SPOT)
			{
				float position[4];
				position[0] = l->getPosition().x;
				position[1] = l->getPosition().y;
				position[2] = l->getPosition().z;
				position[3] = 1.f;
				glLightfv(lightIndex[i], GL_POSITION, position);
			}
			if(l->getType() == Light::SPOT)
			{
				float spotDirection[3];
				spotDirection[0] = l->getSpotDirection().x;
				spotDirection[1] = l->getSpotDirection().y;
				spotDirection[2] = l->getSpotDirection().z;
				glLightfv(lightIndex[i], GL_SPOT_DIRECTION, spotDirection);
				glLightf(lightIndex[i], GL_SPOT_EXPONENT, l->getSpotExponent());
				glLightf(lightIndex[i], GL_SPOT_CUTOFF, l->getSpotCutoff());
			}

			float diffuse[4];
			diffuse[0] = l->getDiffuseColor().x;
			diffuse[1] = l->getDiffuseColor().y;
			diffuse[2] = l->getDiffuseColor().z;
			diffuse[3] = 1.f;
			glLightfv(lightIndex[i], GL_DIFFUSE, diffuse);

			float ambient[4];
			ambient[0] = l->getAmbientColor().x;
			ambient[1] = l->getAmbientColor().y;
			ambient[2] = l->getAmbientColor().z;
			ambient[3] = 0;
			glLightfv(lightIndex[i], GL_AMBIENT, ambient);

			float specular[4];
			specular[0] = l->getSpecularColor().x;
			specular[1] = l->getSpecularColor().y;
			specular[2] = l->getSpecularColor().z;
			specular[3] = 0;
			glLightfv(lightIndex[i], GL_SPECULAR, specular);
		}
	}
}

void GLRenderContext::setMaterial(Material *m)
{
	if(m!=0)
	{
		float diffuse[4];
		diffuse[0] = m->getDiffuse().x;
		diffuse[1] = m->getDiffuse().y;
		diffuse[2] = m->getDiffuse().z;
		diffuse[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

		float ambient[4];
		ambient[0] = m->getAmbient().x;
		ambient[1] = m->getAmbient().y;
		ambient[2] = m->getAmbient().z;
		ambient[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

		float specular[4];
		specular[0] = m->getSpecular().x;
		specular[1] = m->getSpecular().y;
		specular[2] = m->getSpecular().z;
		specular[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m->getShininess());

		Texture *tex = m->getTexture();
		if(tex!=0)
		{
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, tex->getId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if(m->getShader()!=0)
		{
			m->getShader()->use();
		}
	}
}