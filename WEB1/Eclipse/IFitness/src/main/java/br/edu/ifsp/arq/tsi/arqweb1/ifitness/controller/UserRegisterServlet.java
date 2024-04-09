package br.edu.ifsp.arq.tsi.arqweb1.ifitness.controller;

import java.io.IOException;
import java.time.LocalDate;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import br.edu.ifsp.arq.tsi.arqweb1.ifitness.model.Gender;
import br.edu.ifsp.arq.tsi.arqweb1.ifitness.model.User;
import br.edu.ifsp.arq.tsi.arqweb1.ifitness.model.util.user.Encryptor;
import br.edu.ifsp.arq.tsi.arqweb1.ifitness.model.util.user.UserWriter;

@WebServlet("/userRegister")
public class UserRegisterServlet extends HttpServlet {

	private static final long serialVersionUID = 1L;

	public UserRegisterServlet() {
		super();
	}

	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		String name = req.getParameter("name");
		String email = req.getParameter("email");
		String password = req.getParameter("password");
		var profilePictureFile = req.getParameter("profilePictureFile");
		String birthDate = req.getParameter("dateOfBirth");
		String gender = req.getParameter("gender");
		
		System.out.println("Profile Picture: " + profilePictureFile.getClass());

		User user = new User(name, email, Encryptor.encrypt(password), LocalDate.parse(birthDate), Gender.valueOf(gender));

		RequestDispatcher dispatcher = null;

		if (UserWriter.write(user)) {
			req.setAttribute("result", "registered");
			dispatcher = req.getRequestDispatcher("./login.jsp");
		} else {
			req.setAttribute("result", "notRegistered");
			dispatcher = req.getRequestDispatcher("./user-register.jsp");
		}
		
		dispatcher.forward(req, resp);

	}
}
